/* udp.h
 */
#ifndef _SENDIP_UDP_H
#define _SENDIP_UDP_H

/* UDP HEADER
 */
typedef struct {
  u_int16_t     source;
  u_int16_t     dest;
  u_int16_t     len;
  u_int16_t     check;
} udp_header;

/* Defines for which parts have been modified
 */
#define UDP_MOD_SOURCE  1
#define UDP_MOD_DEST    1<<1
#define UDP_MOD_LEN     1<<2
#define UDP_MOD_CHECK   1<<3

#endif  /* _SENDIP_UDP_H */
