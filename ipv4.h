/* ip.h
 */
#ifndef _SENDIP_IP_H
#define _SENDIP_IP_H

/* IP HEADER
 * Taken from glibc 2.2, and modified
 */
typedef struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int header_len:4;
	unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int version:4;
	unsigned int header_len:4;
#else
#  error "Please fix <bits/endian.h>"
#endif
	u_int8_t tos;
	u_int16_t tot_len;
	u_int16_t id;
// FreeBSD automatically hotnses this, for some reason.
#if defined(__FreeBSD__) || defined(__FreeBSD)
	u_int16_t res:1;
	u_int16_t df:1;
	u_int16_t mf:1;
	u_int16_t frag_off:13;
#define IP_SET_FRAGOFF(ip,v) (ip)->frag_off=(v)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
	u_int16_t frag_off1:5;
	u_int16_t mf:1;
	u_int16_t df:1;
	u_int16_t res:1;
	u_int16_t frag_off2:8;
#define IP_SET_FRAGOFF(ip,v) {(ip)->frag_off1=((v)&0x1F00)>>8;(ip)->frag_off2=(v)&0x00FF;}
#elif __BYTE_ORDER == __BIG_ENDIAN
	u_int16_t res:1;
	u_int16_t df:1;
	u_int16_t mf:1;
	u_int16_t frag_off:13;
#define IP_SET_FRAGOFF(ip,v) (ip)->frag_off=(v)
#else
#  error "Please fix <bits/endian.h>"
#endif
	u_int8_t ttl;
	u_int8_t protocol;
	u_int16_t check;
	u_int32_t saddr;
	u_int32_t daddr;
} ip_header;

/* Defines for which parts have been modified
 */
#define IP_MOD_HEADERLEN  (1)
#define IP_MOD_VERSION    (1<<1)
#define IP_MOD_TOS        (1<<2)
#define IP_MOD_TOTLEN     (1<<3)
#define IP_MOD_ID         (1<<4)
#define IP_MOD_RES        (1<<5)
#define IP_MOD_DF         (1<<6)
#define IP_MOD_MF         (1<<7)
#define IP_MOD_FRAGOFF    (1<<8)
#define IP_MOD_TTL        (1<<9)
#define IP_MOD_PROTOCOL   (1<<10)
#define IP_MOD_CHECK      (1<<11)
#define IP_MOD_SADDR      (1<<12)
#define IP_MOD_DADDR      (1<<13)

#endif  /* _SENDIP_IP_H */
