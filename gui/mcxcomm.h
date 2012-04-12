#ifndef MCXCOMM_INCLUDED
#define MCXCOMM_INCLUDED

#include <sys/types.h>

typedef unsigned char u8;

enum {
  DATA_LEN = 13,
};

enum {
  STX = 0x2,
  ETX = 0x3,
  EOT = 0x4,
  ENQ = 0x5,
  ACK = 0x6,
  NAK = 0x15,
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

extern bool mcxcomm_init();
extern void mcxcmd_set(msg *msg, u8 item, const u8 *data, size_t len);
extern void mcxcmd_get(msg *msg, u8 item, u8 subitem = 0, u8 subsubitem = 0);
extern void mcxcmd_dump(char *buf, size_t len, const msg *msg);

extern bool mcxcomm_connect(const char *filename);
extern void mcxcomm_disconnect();
extern bool mcxcomm_send_enq();
extern bool mcxcomm_send_ack();
extern bool mcxcomm_send_msg(const msg& msg);
extern bool mcxcomm_recv(msg *msg, unsigned int timeout_ms, bool *err);

#endif
