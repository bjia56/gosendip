/* rip.c - RIP-1 and -2 code for sendip
 * Taken from code by Richard Polton <Richard.Polton@msdw.com>
 * ChangeLog since 2.0 release:
 * 02/12/2001 Only check 1 layer for enclosing UDP header
 * 21/08/2002 Off-by-one fix in -re handling that caused bad things to happen
 * 21/08/2002 htons() and htonl() added where needed
 * ChangeLog since 2.2 release:
 * 24/11/2002 make it compile on archs that care about alignment
 * ChangeLog since 2.5 release:
 * 26/10/2004 fix bug with multiple -re options (found by several people)
 * 28/10/2004 fix -ra (thanks to sharmily.anantaraman@conexant.com)
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "sendip_module.h"
#include "rip.h"

/* Options
 */
sendip_option rip_opts[] = {
	{"v",1,"RIP version","2"},
	{"c",1,
	 "RIP command (1=request, 2=response, 3=traceon (obsolete), 4=traceoff (obsolete), 5=poll (undocumented), 6=poll entry (undocumented)","1"},
	{"e",1,"Add a RIP entry.  Format is: Address family:route tag:address:subnet mask:next hop:metric","2:0:0.0.0.0:255.255.255.0:0.0.0.0:16, any option my be left out to use the default"},
	{"a",1,"Add RIP auth entry.  Format is: AuthType:Password, AuthType may be omitted for default basic auth",NULL},
	{"d",0,"RIP default request - get router's entire routing table; do not use any other RIP options on this RIP header",NULL},
	{"r",1,"RIP reserved field","0"}
};


/* Character that identifies our options
 */
const char rip_opt_char='r';

sendip_data *rip_initialize(void) {
	sendip_data *ret = malloc(sizeof(sendip_data));
	rip_header *rip = malloc(sizeof(rip_header));
	memset(rip,0,sizeof(rip_header));
	ret->alloc_len = sizeof(rip_header);
	ret->data = (void *)rip;
	ret->modified=0;
	return ret;
}

bool rip_set_addr(char *hostname, sendip_data *pack) {
	/* RIP doesn't need to set any addresses based on hostname */
	return TRUE;
}

bool rip_do_opt(const char *opt, char *arg, sendip_data *pack) {
	rip_header *rippack = (rip_header *)pack->data;
	rip_options *ripopt;
	char *p, *q;
	switch(opt[1]) {
	case 'v': /* version */
		rippack->version = (u_int8_t)strtoul(arg, (char **)0, 0);
		pack->modified |= RIP_MOD_VERSION;
		break;
	case 'c': /* command */
		rippack->command = (u_int8_t)strtoul(arg, (char **)0, 0);
		pack->modified |= RIP_MOD_COMMAND;
		break;
	case 'a': /* authenticate */
		if(RIP_NUM_ENTRIES(pack) != 0) {
			usage_error("Warning: a real RIP-2 packet only has authentication as the first entry.\n");
		}
		RIP_ADD_ENTRY(pack);
		ripopt = RIP_OPTION(pack);
		memset(ripopt,0,sizeof(rip_options));
		ripopt->addressFamily=0xFFFF;
		p=q=arg;
		/* TODO: if arg is malformed, this could segfault */
		while(*(q++)!=':') {} /* do nothing */; *(--q)='\0';
		ripopt->routeTagOrAuthenticationType=htons((p==q)?2:(u_int16_t)strtoul(p, (char **)0,0));
		p=++q;
		if(strlen(p) > 16) {
		  usage_error("Warning: RIP passord cannot be longer than 16 characters.\n");
		}
		strncpy((char *)&(ripopt->address),p,16);
		break;
	case 'e': /* rip entry */
		if(RIP_NUM_ENTRIES(pack)==25) {
			usage_error("Warning: a real RIP packet contains no more than 25 entries.\n");
		}
		RIP_ADD_ENTRY(pack);
		ripopt = RIP_OPTION(pack);
		p=q=arg;
		/* TODO: if arg is malformed, this could segfault */
		while(*(q++)!=':') {} /* do nothing */; *(--q)='\0';
		ripopt->addressFamily= htons((p==q)?2:(u_int16_t)strtoul(p, (char **)0, 0));
		p=++q; while(*(q++)!=':') {} /* do nothing */; *(--q)='\0';
		ripopt->routeTagOrAuthenticationType=htons((p==q)?0:(u_int16_t)strtoul(p, (char **)0,0));
		p=++q; while(*(q++)!=':') {} /* do nothing */; *(--q)='\0';
		ripopt->address=(p==q)?inet_addr("0.0.0.0"):inet_addr(p);
		p=++q; while(*(q++)!=':') {} /* do nothing */; *(--q)='\0';
		ripopt->subnetMask=(p==q)?inet_addr("255.255.255.0"):inet_addr(p);
		p=++q; while(*(q++)!=':') {} /* do nothing */; *(--q)='\0';
		ripopt->nextHop=(p==q)?inet_addr("0.0.0.0"):inet_addr(p);
		p=++q; while(*(q++)!='\0') {} /* do nothing */; *(--q)='\0';
		ripopt->metric=htonl((p==q)?16:(u_int32_t)strtoul(p,(char **)0, 0));
		break;
	case 'd': /* default request */
		if(RIP_NUM_ENTRIES(pack) != 0) {
			usage_error("Warning: a real RIP-1 or -2 packet does not have any entries in a default request.\n");
		}
		RIP_ADD_ENTRY(pack);
		ripopt=RIP_OPTION(pack);
		rippack->command = (u_int8_t)1;
		ripopt->addressFamily = (u_int16_t)0;
		ripopt->routeTagOrAuthenticationType = (u_int16_t)0;
		ripopt->address=inet_addr("0.0.0.0");
		ripopt->subnetMask=inet_addr("0.0.0.0");
		ripopt->nextHop=inet_addr("0.0.0.0");
		ripopt->metric=htons((u_int16_t)16);
		break;
	case 'r': /* set reserved field */
		rippack->res = (u_int16_t)strtoul(arg, (char **)0, 0);
		pack->modified |= RIP_MOD_RESERVED;
		break;
	default:
	  usage_error("Unrecognized option opt");
	  return FALSE;
	}
	return TRUE;

}

bool rip_finalize(char *hdrs, sendip_data *headers[], sendip_data *data,
				  sendip_data *pack) {
	if(hdrs[strlen(hdrs)-1] != 'u') {
		usage_error("Warning: RIP should be contained in a UDP packet\n");
	}

	return TRUE;
}

int rip_num_opts(void) {
	return sizeof(rip_opts)/sizeof(sendip_option);
}
sendip_option *rip_get_opts(void) {
	return rip_opts;
}
char rip_get_optchar(void) {
	return rip_opt_char;
}
