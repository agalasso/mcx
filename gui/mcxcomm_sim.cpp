#include "mcxcomm.h"

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
# include "wx/wx.h"
#endif

enum { NRESP = 10 };
static msg s_buf[NRESP];
static int s_rdpos = 0;
static int s_wrpos = 0;
static int s_empty = 1;
static wxMutex s_buflock;

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
  return true;
}

bool
mcxcomm_connect(const char *filename)
{
  // todo
  return true;
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

bool
mcxcomm_send_msg(const msg& cmd)
{
  // simulator sends ack, then response
  msg rsp;
  rsp.stx = ACK;
  _put(&rsp);
  rsp = cmd;
  _put(&rsp);
}

bool
mcxcomm_recv(msg *msg, unsigned int timeout_ms)
{
  bool ok = _get(msg);
  if (ok)
    return ok;
  wxMilliSleep(timeout_ms / 2);
  return _get(msg);
}

