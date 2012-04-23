#include "mcxcomm.h"

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
# include "wx/wx.h"
#endif

#include "crc16.h"

#include <stdlib.h>

enum { NRESP = 10 };
static msg s_buf[NRESP];
static int s_rdpos = 0;
static int s_wrpos = 0;
static int s_empty = 1;
static wxMutex s_buflock;

#ifdef __WXMSW__
static unsigned int s_randbuf;
#else
static struct random_data s_randbuf;
#endif

static void
_put(const msg *msg)
{
  wxMutexLocker l(s_buflock);
  s_buf[s_wrpos] = *msg;
  s_wrpos = (s_wrpos + 1) % NRESP;
  s_empty = 0;
}

static bool
_get(msg *msg)
{
  wxMutexLocker l(s_buflock);
  if (s_empty)
    return false;
  *msg = s_buf[s_rdpos];
  s_rdpos = (s_rdpos + 1) % NRESP;
  if (s_rdpos == s_wrpos)
    s_empty = 1;
  return true;
}

bool
mcxcomm_init()
{
#ifdef __WXMSW__
    s_randbuf = time(0);
#else
    srandom_r(time(0), &s_randbuf);
#endif
    return true;
}

bool
mcxcomm_connect(const char *filename)
{
    // todo
    return true;
}

void
mcxcomm_disconnect()
{
}

bool
mcxcomm_send_enq()
{
    // todo - simulated
    //    simulator sends ack
    msg msg;
    msg.stx = ACK;
    _put(&msg);
}

bool
mcxcomm_send_ack()
{
    // nothing
}

static int s_cnt = 1;
static int s_drop_ack_pct;
static int s_drop_rsp_pct;

bool
mcxcomm_send_msg(const msg& cmd)
{
    // simulator sends ack, then response
    msg rsp;

    int32_t val;
#ifdef __WXMSW__
    val = rand();
#else
    random_r(&s_randbuf, &val);
#endif

    if (s_cnt > 0) {
        if (--s_cnt == 0) {
            s_drop_ack_pct = 0;
            s_drop_rsp_pct = 0;
        }
    }
    if (val < RAND_MAX / 100 * (100 - s_drop_ack_pct)) {
        rsp.stx = ACK;
        _put(&rsp);
    }
    if (val < RAND_MAX / 100 * (100 - s_drop_rsp_pct)) {
        rsp = cmd;

        rsp.cmdrsp = 0xA0; // RSP_OK

//        if (cmd.ctrl == 0x45 && cmd.data[0] == 0x1)
//            rsp.data[4] = 12; // senseUp 128x
        if (cmd.ctrl == 0x45 && cmd.data[0] == 0x1)
            rsp.data[7] = 4; // agcManLevel
        if (cmd.ctrl == 0x45 && cmd.data[0] == 0x3)
            rsp.data[4] = 2; // agcMan

  unsigned short crc = crc16(&rsp.cmdrsp, (unsigned long) &rsp.crc_hi - (unsigned long) &rsp.cmdrsp);
  rsp.crc_hi = (u8) (crc >> 8);
  rsp.crc_lo = (u8) (crc & 0xff);

        _put(&rsp);
    }
}

bool
mcxcomm_recv(msg *msg, unsigned int timeout_ms, bool *err)
{
    *err = false;
    bool ok = _get(msg);
    if (ok)
        return ok;
    wxMilliSleep(timeout_ms / 4);
    return _get(msg);
}
