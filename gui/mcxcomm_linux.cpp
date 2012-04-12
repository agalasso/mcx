#include "mcxcomm.h"

bool
mcxcomm_init()
{
  return true;
}

bool
mcxcomm_connect(const char *filename)
{
  return false;
}

void
mcxcomm_disconnect()
{
}

bool
mcxcomm_send_enq()
{
  return false;
}

bool
mcxcomm_send_ack()
{
  return false;
}

bool
mcxcomm_send_msg(const msg& msg)
{
  return false;
}

bool
mcxcomm_recv(msg *msg, unsigned int timeout_ms, bool *err)
{
  *err = true;
  return false;
}

# if 0
#include "crc16.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DBG printf

static unsigned short
crc(const msg *msg)
{
  return crc16(&msg->cmdrsp, (unsigned long) &msg->crc_hi - (unsigned long) &msg->cmdrsp);
}

struct mcx {
  int fd;
};
static mcx mcx;

static void
fatal(const char *s)
{
  // TODO
  exit(1);
}

static int
_open_fd(const char *devname)
{
  DBG("open fd\n");

  int fd = open(devname, O_RDWR | O_NONBLOCK);
  if (fd == -1) fatal("open failed");

  DBG("set attrs\n");

  struct termios ti;
  int ret = tcgetattr(fd, &ti);
  if (ret == -1) fatal("tcgetattr failed");

  cfmakeraw(&ti);         // includes CS8, clears PARENB
  cfsetspeed(&ti, B9600); // 9600 baud
  ti.c_cflag |= CLOCAL;   // no hw flow control

  ret = tcsetattr(fd, TCSAFLUSH, &ti);
  if (ret == -1) fatal("tcsetattr failed");

  return fd;
}

static bool
_send_enq()
{
  DBG("send enq\n");
  u8 const buf = ENQ;
  ssize_t ret = TEMP_FAILURE_RETRY(write(mcx.fd, &buf, 1));
  DBG("send enq ret %ld err %d\n", ret, ret == -1 ? errno : 0);
  return ret == 1;
}

static bool
_wait_for_ack()
{
  fd_set rfd;
  FD_ZERO(&rfd);
  FD_SET(mcx.fd, &rfd);

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  DBG("select 1s timeout\n");
  int sret = TEMP_FAILURE_RETRY(select(mcx.fd + 1, &rfd, 0, 0, &tv));
  DBG("select ret %d err %d\n", sret, sret == -1 ? errno : 0);

  if (sret == 0) {
    DBG("timeout waiting for ack\n");
    return false;
  }

  u8 ch;
  ssize_t ret = TEMP_FAILURE_RETRY(read(mcx.fd, &ch, 1));
  DBG("read ret %ld err %d ch = 0x%x\n", ret, ret == -1 ? errno : 0, ret == 1 ? (unsigned int) ch : 0);

  if (ch == ACK) {
    DBG("got ack\n");
    return true;
  }

  DBG("expecting ack (0x6), got 0x%x\n", (unsigned int) ch);
}

static void
_write_n(int fd, const void *buf, size_t len)
{
  DBG("writing %zu bytes\n", len);
  const char *p = (const char *) buf;
  size_t rem = len;
  while (rem > 0) {
    ssize_t ret = TEMP_FAILURE_RETRY(write(fd, p, rem));
    if (ret == -1) fatal("write");
    rem -= ret;
    p += ret;
  }
}

static void
_send_ack()
{
  DBG("sending ack\n");
  char ch = ACK;
  _write_n(mcx.fd, &ch, 1);
  DBG("sent ack\n");
}

static bool
_read_response(msg *rsp)
{
  size_t rem = sizeof(*rsp);
  char *dst = (char *) rsp;

  while (true) {
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(mcx.fd, &rfd);

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    DBG("select 10s timeout\n");
    int sret = TEMP_FAILURE_RETRY(select(mcx.fd + 1, &rfd, 0, 0, &tv));
    DBG("select ret %d err %d\n", sret, sret == -1 ? errno : 0);

    if (sret == 0) {
      DBG("timeout waiting for response\n");
      return false;
    }

    DBG("reading %zu\n", rem);
    ssize_t ret = TEMP_FAILURE_RETRY(read(mcx.fd, dst, rem));
    DBG("read ret %ld err %d\n", ret, ret == -1 ? errno : 0);
    if (ret == -1)
      fatal("read failed\n");
    rem -= ret;
    if (rem == 0)
      return true;

    dst += ret;
  }
}

static bool
_send_cmd(const msg *cmd, msg *rsp)
{
  _write_n(mcx.fd, cmd, sizeof(*cmd));
  if (!_wait_for_ack())
    fatal("did not get ack!");

  bool ok = _read_response(rsp);
  _send_ack();

  return ok;
}

static void
usage()
{
  exit(1); // todo
}

void
_dump_msg(const char *s, const msg *msg)
{
  unsigned short crcc = crc(msg);
  unsigned short crcm = (((unsigned short) msg->crc_hi) << 8) | msg->crc_lo;

  char buf[128];
  mcxcmd_dump(buf, sizeof(buf), msg);

  printf("%s: %s %s\n", s, buf, crcc == crcm ? "" : "*MISMATCH");
}

int
zzmain(int argc, char *argv[])
{
  if (argc < 2)
    usage();

  const char *devname = argv[1];

  mcxcomm_init();

  mcx.fd = _open_fd(devname);

  bool ok = _send_enq();
  if (!ok) fatal("send enq failed");

  if (!_wait_for_ack())
    fatal("wait for ack failed");

  if (0) {
    // try sending text label
    u8 buf[] = {
      0x1, // title chars
      0x18, 0x15, 0x1c, 0x1c, 0x0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    };

    msg cmd;
    mcxcmd_set(&cmd, 0x10, &buf[0], sizeof(buf));
    _dump_msg("sending cmd", &cmd);

    msg rsp;
    ok = _send_cmd(&cmd, &rsp);
    if (!ok)
      fatal("cmd failed\n");

    _dump_msg("got resp", &rsp);
  }

  if (0) {
    // send "title on"
    u8 buf[] = {
      0x0, // title on/off
      0x1, // on
    };

    msg cmd;
    mcxcmd_set(&cmd, 0x10, &buf[0], sizeof(buf));
    _dump_msg("sending cmd", &cmd);

    msg rsp;
    ok = _send_cmd(&cmd, &rsp);
    if (!ok)
      fatal("cmd failed\n");

    _dump_msg("got resp", &rsp);
  }

  if (1) {
    // send "title off"
    u8 buf[] = {
      0x0, // title on/off
      0x0, // off
    };

    msg cmd;
    mcxcmd_set(&cmd, 0x10, &buf[0], sizeof(buf));
    _dump_msg("sending cmd", &cmd);

    msg rsp;
    ok = _send_cmd(&cmd, &rsp);
    if (!ok)
      fatal("cmd failed\n");

    _dump_msg("got resp", &rsp);
  }
}
#endif
