/* ipv6.c - sendip IPv6 code
 * Taken from code by Antti Tuominen <ajtuomin@tml.hut.fi>
 * ChangeLog since 2.0 release:
 * 09/08/2002 Setting src/dst now works (Pekka Savola <pekkas@netcore.fi>)
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "sendip_module.h"
#include "ipv6.h"

/* Options
 */
sendip_option ipv6_opts[] = {
	{"f",1,"IPv6 flow ID","32"},
	{"t",1,"IPv6 traffic class","0"},
	{"l",1,"IPv6 payload length","Correct"},
	{"n",1,"IPv6 next header","IPPROTO_NONE"},
	{"h",1,"IPv6 hop limit","32"},
	{"v",1,"IP version (you probably don't want to change this"},
	{"p",1,"IPv6 priority","0"},
	{"s",1,"IPv6 source address","::1"},
	{"d",1,"IPv6 destination address","Correct"}
};

/* Character that identifies our options
 */
const char ipv6_opt_char='6';

sendip_data *ipv6_initialize(void) {
	sendip_data *ret = malloc(sizeof(sendip_data));
	ipv6_header *ipv6 = malloc(sizeof(ipv6_header));
	memset(ipv6,0,sizeof(ipv6_header));
	ret->alloc_len = sizeof(ipv6_header);
	ret->data = (void *)ipv6;
	ret->modified=0;
	return ret;
}

bool ipv6_set_addr(char *hostname, sendip_data *pack) {
	ipv6_header *ipv6 = (ipv6_header *)pack->data;
	struct hostent *host = gethostbyname2(hostname,AF_INET6);
	if(!(pack->modified & IPV6_MOD_SRC)) {
		ipv6->ip6_src = in6addr_loopback;
	}
	if(!(pack->modified & IPV6_MOD_DST)) {
		if(host==NULL) return FALSE;
		if(host->h_length != sizeof(ipv6->ip6_dst)) {
			fprintf(stderr,"IPV6 destination address is the wrong size!!!");
			return FALSE;
		}
		memcpy(&(ipv6->ip6_dst),host->h_addr,host->h_length);
	}
	return TRUE;
}

bool ipv6_do_opt(char *opt, char *arg, sendip_data *pack) {
	ipv6_header *hdr = (ipv6_header *)pack->data;
	struct in6_addr addr;

	switch(opt[1]) {
	case 'f':
		/* TODO : This looks byte-order dependant */
		hdr->ip6_flow |= htonl((u_int32_t)strtoul(arg, (char **)NULL, 0) & 0xFFF00000);
		pack->modified |= IPV6_MOD_FLOW;
		break;
	case 'v':
		hdr->ip6_vfc &= 0x0F;
		hdr->ip6_vfc |= (u_int8_t)(strtoul(arg, (char **)NULL, 0) &0x0F) << 4;
		pack->modified |= IPV6_MOD_VERSION;
		break;
	case 'p':
		hdr->ip6_vfc &= 0xF0;
		hdr->ip6_vfc |= (u_int8_t)strtoul(arg, (char **)NULL, 0) & 0x0F;
		pack->modified |= IPV6_MOD_PRIORITY;
		break;
	case 't':
		/* TODO : This looks byte-order dependant */
		hdr->ip6_flow |= htonl(((u_int32_t)strtoul(arg, (char **)NULL, 0) << 20) & 0x0F000000);
		pack->modified |= IPV6_MOD_FLOW;
		break;
	case 'l':
		hdr->ip6_plen = htons((u_int16_t)strtoul(arg, (char **)NULL, 0));
		pack->modified |= IPV6_MOD_PLEN;
		break;
	case 'h':
		hdr->ip6_hlim = (u_int8_t)strtoul(arg, (char **)NULL, 0);
		pack->modified |= IPV6_MOD_HLIM;
		break;
	case 'n':
		hdr->ip6_nxt = (u_int8_t)strtoul(arg, (char **)NULL, 0);
		pack->modified |= IPV6_MOD_NXT;
		break;
	case 's':
		if (inet_pton(AF_INET6, arg, &addr)) {
			memcpy(&hdr->ip6_src, &addr, sizeof(struct in6_addr));
		}
		pack->modified |= IPV6_MOD_SRC;
		break;
	case 'd':
		if (inet_pton(AF_INET6, arg, &addr)) {
			memcpy(&hdr->ip6_dst, &addr, sizeof(struct in6_addr));
		}
		pack->modified |= IPV6_MOD_DST;
		break;
	}
	return TRUE;

}

bool ipv6_finalize(char *hdrs, sendip_data *headers[], sendip_data *data,
				sendip_data *pack) {
	ipv6_header *ipv6 = (ipv6_header *)pack->data;

	if(!(pack->modified&IPV6_MOD_VERSION)) {
		ipv6->ip6_vfc &= 0x0F;
		ipv6->ip6_vfc |= (6 << 4);
	}
	if(!(pack->modified&IPV6_MOD_PLEN)) {
		ipv6->ip6_plen = htons(data->alloc_len);
	}
	if(!(pack->modified&IPV6_MOD_NXT)) {
		ipv6->ip6_nxt = (u_int8_t)IPPROTO_NONE;
	}
	if(!(pack->modified&IPV6_MOD_HLIM)) {
		ipv6->ip6_hlim = 32;
	}

	return TRUE;
}

int ipv6_num_opts(void) {
	return sizeof(ipv6_opts)/sizeof(sendip_option);
}
sendip_option *ipv6_get_opts(void) {
	return ipv6_opts;
}
char ipv6_get_optchar(void) {
	return ipv6_opt_char;
}
