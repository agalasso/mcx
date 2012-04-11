#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "crc16.h"

#define DBG printf

typedef unsigned char u8;

enum {
  STX = 0x2,
  ETX = 0x3,
  ENQ = 0x5,
  ACK = 0x6,

  DATA_LEN = 13,

  CMD_SET = 0x21,
  CMD_GET = 0x31,

  RSP_OK = 0xA0,
  RSP_NE1 = 0xA2,
  RSP_NE2 = 0xA3,
  RSP_FAIL = 0xA4,
  RSP_OTHER = 0xA5,
};

struct msg
{
  u8 stx;                 // [0]
  u8 cmdrsp;              // [1]
  u8 ctrl;                // [2]
  u8 data[DATA_LEN];      // [3..15]
  u8 etx;                 // [16]
  u8 crc_hi;              // [17]
  u8 crc_lo;              // [18]
};

static unsigned short
crc(const msg *msg)
{
  return crc16(&msg->cmdrsp, (unsigned long) &msg->crc_hi - (unsigned long) &msg->cmdrsp);
}

static void
_dump_msg(const char *s, const msg *msg)
{
  unsigned short crcc = crc(msg);
  unsigned short crcm = (((unsigned short) msg->crc_hi) << 8) | msg->crc_lo;

  printf("%s: stx=%02x cmd=%02x ctrl=%02x data=(%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x) etx=%02x crc=%02x%02x %s\n",
	 s, msg->stx, msg->cmdrsp, msg->ctrl,
	 msg->data[0], msg->data[1], msg->data[2],
	 msg->data[3], msg->data[4], msg->data[5],
	 msg->data[6], msg->data[7], msg->data[8],
	 msg->data[9], msg->data[10], msg->data[11], msg->data[12],
	 msg->etx, msg->crc_hi, msg->crc_lo, crcc == crcm ? "" : "*MISMATCH");
}

static void
__init_cmd(msg *msg, u8 cmdrsp, u8 item, const u8 *data, size_t len)
{
  msg->stx = STX;
  msg->cmdrsp = cmdrsp;
  msg->ctrl = item;

  if (len > sizeof(msg->data))
    len = sizeof(msg->data);
  unsigned int i;
  for (i = 0; i < len; i++)
    msg->data[i] = data[i];
  for (; i < sizeof(data); i++)
    msg->data[i] = 0;

  msg->etx = ETX;

  unsigned short crc = crc16(&msg->cmdrsp, (unsigned long) &msg->crc_hi - (unsigned long) &msg->cmdrsp);
  msg->crc_hi = (u8) (crc >> 8);
  msg->crc_lo = (u8) (crc & 0xff);
}

static void
_init_cmd_set(msg *msg, u8 item, const u8 *data, size_t len)
{
  __init_cmd(msg, CMD_SET, item, data, len);
}

static void
_init_cmd_get(msg *msg, u8 item)
{
  __init_cmd(msg, CMD_GET, item, 0, 0);
}

static void
usage()
{
  fprintf(stderr, "usage: hello DEVICE\n");
  exit(1);
}

static void
fatal(const char *s)
{
  fprintf(stderr, "%s\n", s);
  exit(1);
}

struct mcx {
  int fd;
};
static mcx mcx;

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
init_mcx()
{
}

int
main(int argc, char *argv[])
{
  if (argc < 2)
    usage();

  const char *devname = argv[1];

  init_mcx();

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
    _init_cmd_set(&cmd, 0x10, &buf[0], sizeof(buf));
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
    _init_cmd_set(&cmd, 0x10, &buf[0], sizeof(buf));
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
    _init_cmd_set(&cmd, 0x10, &buf[0], sizeof(buf));
    _dump_msg("sending cmd", &cmd);

    msg rsp;
    ok = _send_cmd(&cmd, &rsp);
    if (!ok)
      fatal("cmd failed\n");

    _dump_msg("got resp", &rsp);
  }
}
