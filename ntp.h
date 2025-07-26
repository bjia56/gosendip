/* ntp.h
 */
#ifndef _SENDIP_NTP_H
#define _SENDIP_NTP_H

typedef struct {
	u_int32_t intpart;
	u_int32_t fracpart;
} ntp_ts;

/* NTP HEADER
 */
typedef struct {
	/* TODO BYTEORDER!!! */
	u_int8_t leap:2;
	u_int8_t status:6;
	u_int8_t type;
	/* END TODO */

	u_int16_t precision;
	u_int32_t error;
	u_int32_t drift;
	union {
		u_int32_t ipaddr;
		char id[4];
	} reference;
	ntp_ts reference_ts;
	ntp_ts originate_ts;
	ntp_ts receive_ts;
	ntp_ts transmit_ts;
} ntp_header;

/* Defines for which parts have been modified
 */
#define NTP_MOD_LEAP      (1)
#define NTP_MOD_STATUS    (1<<1)
#define NTP_MOD_TYPE      (1<<2)
#define NTP_MOD_PRECISION (1<<3)
#define NTP_MOD_ERROR     (1<<4)
#define NTP_MOD_DRIFT     (1<<5)
#define NTP_MOD_REF       (1<<6)
#define NTP_MOD_REFERENCE (1<<7)
#define NTP_MOD_ORIGINATE (1<<8)
#define NTP_MOD_RECEIVE   (1<<9)
#define NTP_MOD_TRANSMIT  (1<<10)

#endif  /* _SENDIP_NTP_H */
