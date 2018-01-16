/*
 * MallinCam Control
 * Copyright (C) 2012-2018 Andrew Galasso
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
extern int mcxcmd_validate(const msg *msg);
extern void mcxcmd_dump(char *buf, size_t len, const msg *msg);

extern bool mcxcomm_connect(const char *filename);
extern void mcxcomm_disconnect();
extern bool mcxcomm_send_enq();
extern bool mcxcomm_send_ack();
extern bool mcxcomm_send_msg(const msg& msg);
extern bool mcxcomm_recv(msg *msg, unsigned int timeout_ms, bool *err);

#endif
