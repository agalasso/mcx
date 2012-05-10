#include "wx/wxprec.h"
#ifndef WX_PRECOMP
# include "wx/wx.h"
#endif

#include "mcxcomm.h"
#include "mcx.h"

#include <termios.h>
#include <errno.h>
#include <sys/select.h>

static int
_open_device(const char *devname)
{
    VERBOSE("open fd %s", devname);

    int fd = open(devname, O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        WARN("open failed");
        return fd;
    }

    VERBOSE("set attrs");

    struct termios ti;
    int ret = tcgetattr(fd, &ti);
    if (ret == -1) {
        WARN("tcgetattr failed");
        close(fd);
        return -1;
    }

    cfmakeraw(&ti);         // includes CS8, clears PARENB
    cfsetspeed(&ti, B9600); // 9600 baud
    ti.c_cflag |= CLOCAL;   // no hw flow control

    ret = tcsetattr(fd, TCSAFLUSH, &ti);
    if (ret == -1) {
        WARN("tcsetattr failed");
        close(fd);
        return -1;
    }

    return fd;
}

struct mcxcomm {
    int fd;
};
static mcxcomm s_mcxc;

bool
mcxcomm_init()
{
    VERBOSE("%s", __FUNCTION__);

    s_mcxc.fd = -1;

    return true;
}

bool
mcxcomm_connect(const char *device)
{
    VERBOSE("%s %s", __FUNCTION__, device);

    if (s_mcxc.fd != -1)
        close(s_mcxc.fd);

    s_mcxc.fd = _open_device(device);

    VERBOSE("%s returns %d", "_open_device", s_mcxc.fd);

    return s_mcxc.fd != -1;
}

void
mcxcomm_disconnect()
{
    VERBOSE("%s",__FUNCTION__);
    if (s_mcxc.fd != -1) {
        close(s_mcxc.fd);
        s_mcxc.fd = -1;
    }
}

static bool
_send1(char val)
{
    VERBOSE("%s 0x%x",__FUNCTION__,(unsigned int) val);

    ssize_t n = write(s_mcxc.fd, &val, sizeof(val));
    return n == sizeof(val);
}


static bool
_write_n(const void *buf, size_t len)
{
    const char *p = (const char *) buf;
    size_t rem = len;
    while (rem > 0) {
        ssize_t ret = TEMP_FAILURE_RETRY(write(s_mcxc.fd, p, rem));
        if (ret == -1)
            return false;
        p += ret;
        rem -= ret;
    }
    return true;
}

bool
mcxcomm_send_enq()
{
    return _send1(ENQ);
}

bool
mcxcomm_send_ack()
{
    return _send1(ACK);
}

bool
mcxcomm_send_msg(const msg& cmd)
{
    VERBOSE("%s %02x %02x %02x %02x %02x %02x", __FUNCTION__, cmd.stx,
            cmd.cmdrsp, cmd.ctrl, cmd.data[0], cmd.data[1], cmd.data[2]);

    return _write_n(&cmd, sizeof(cmd));
}

static bool
_rdwait(int fd, unsigned int timeout_ms, bool *err)
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = TEMP_FAILURE_RETRY(select(fd + 1, &fds, NULL, NULL, &tv));
    if (ret == -1) {
        *err = true;
        WARN("%s: select: %s",__FUNCTION__, strerror(errno));
        return false;
    }
    else if (ret == 0) { // timeout
        *err = false;
        return false;
    }
    *err = false;
    return true;
}

bool
mcxcomm_recv(msg *msg, unsigned int timeout_ms, bool *err)
{
    int const fd = s_mcxc.fd;

    bool rdy = _rdwait(fd, timeout_ms, err);
    if (!rdy)
        return false;

    ssize_t ret = TEMP_FAILURE_RETRY(read(fd, &msg->stx, 1));

    if (ret == 0) { // should never happen
        WARN("%s: could not read initial byte",__FUNCTION__);
        *err = true;
        return false;
    }

    if (ret == -1) {
        *err = true;
        WARN("%s fail reading 1 byte: %s",__FUNCTION__, strerror(errno));
        return false;
    }

    if (msg->stx != STX) {
        *err = false;
        return true;
    }

    // keep reading until timeout or we get a full message. allow 1500 ms
    // between chunks. this matches what we do with the Windows version.
    size_t rem = sizeof(*msg) - 1;
    char *p = (char *) &msg->cmdrsp;
    while (rem > 0) {
        bool rdy = _rdwait(fd, 1500, err);
        if (!rdy) {
            WARN("%s timed-out reading partial message! recvd=%zu", __FUNCTION__,
                 p - (char *) msg);
            return false;
        }
        ssize_t ret = TEMP_FAILURE_RETRY(read(fd, p, rem));
        if (ret == -1) {
            *err = true;
            WARN("%s fail reading msg: %s",__FUNCTION__, strerror(errno));
            return false;
        }
        p += ret;
        rem -= ret;
    }

    *err = false;
    return true;
}
