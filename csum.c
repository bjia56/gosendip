/* csum.c
	Computes the standard internet checksum of a set of data
	from RFC1071
*/

#define __USE_BSD    /* GLIBC */
#define _BSD_SOURCE  /* LIBC5 */
#include <sys/types.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

u_int16_t csum (u_int16_t *packet, int packlen);

/* Checksum a block of data */
u_int16_t csum (u_int16_t *packet, int packlen) {
	register unsigned long sum = 0;

	while (packlen > 1) {
		sum+= *(packet++);
		packlen-=2;
	}

	if (packlen > 0)
		sum += *(unsigned char *)packet;

	/* TODO: this depends on byte order */

	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	return (u_int16_t) ~sum;
}

struct ipv6_pseudo_hdr {
	struct in6_addr source;
	struct in6_addr destination;
	u_int32_t ulp_length;
	u_int32_t  zero: 24,
		nexthdr:  8;
};

u_int16_t ipv6_csum(struct in6_addr *src, struct in6_addr *dst, u_int8_t nxthdr,
		    u_int16_t *packet, int packlen) {

	struct ipv6_pseudo_hdr phdr;
	u_int8_t *tempbuf;

	tempbuf = malloc(sizeof(phdr) + packlen);
	memset(&phdr, 0, sizeof(phdr));
	memcpy(&phdr.source, src, sizeof(struct in6_addr));
	memcpy(&phdr.destination, dst, sizeof(struct in6_addr));
	phdr.ulp_length = htonl(packlen);
	phdr.nexthdr = nxthdr;
	memcpy(tempbuf, &phdr, sizeof(phdr));
	memcpy(tempbuf + sizeof(phdr), packet, packlen);

	return csum((u_int16_t *)tempbuf, sizeof(phdr) + packlen);
}

