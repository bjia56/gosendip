#ifndef _SENDIP_MODULES_H
#define _SENDIP_MODULES_H

#include "sendip_module.h"

/* Module initialization functions */
typedef struct {
    const char *name;
    char optchar;
    sendip_data * (*initialize)(void);
    bool (*do_opt)(const char *optstring, char *optarg, sendip_data *pack);
    bool (*set_addr)(char *hostname, sendip_data *pack);
    bool (*finalize)(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
    int (*num_opts)(void);
    sendip_option * (*get_opts)(void);
    char (*get_optchar)(void);
} module_entry;

/* External declarations for module functions */
/* IPv4 */
sendip_data *ipv4_initialize(void);
bool ipv4_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool ipv4_set_addr(char *hostname, sendip_data *pack);
bool ipv4_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int ipv4_num_opts(void);
sendip_option *ipv4_get_opts(void);
char ipv4_get_optchar(void);

/* IPv6 */
sendip_data *ipv6_initialize(void);
bool ipv6_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool ipv6_set_addr(char *hostname, sendip_data *pack);
bool ipv6_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int ipv6_num_opts(void);
sendip_option *ipv6_get_opts(void);
char ipv6_get_optchar(void);

/* ICMP */
sendip_data *icmp_initialize(void);
bool icmp_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool icmp_set_addr(char *hostname, sendip_data *pack);
bool icmp_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int icmp_num_opts(void);
sendip_option *icmp_get_opts(void);
char icmp_get_optchar(void);

/* TCP */
sendip_data *tcp_initialize(void);
bool tcp_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool tcp_set_addr(char *hostname, sendip_data *pack);
bool tcp_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int tcp_num_opts(void);
sendip_option *tcp_get_opts(void);
char tcp_get_optchar(void);

/* UDP */
sendip_data *udp_initialize(void);
bool udp_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool udp_set_addr(char *hostname, sendip_data *pack);
bool udp_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int udp_num_opts(void);
sendip_option *udp_get_opts(void);
char udp_get_optchar(void);

/* RIP */
sendip_data *rip_initialize(void);
bool rip_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool rip_set_addr(char *hostname, sendip_data *pack);
bool rip_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int rip_num_opts(void);
sendip_option *rip_get_opts(void);
char rip_get_optchar(void);

/* RIPNG */
sendip_data *ripng_initialize(void);
bool ripng_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool ripng_set_addr(char *hostname, sendip_data *pack);
bool ripng_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int ripng_num_opts(void);
sendip_option *ripng_get_opts(void);
char ripng_get_optchar(void);

/* NTP */
sendip_data *ntp_initialize(void);
bool ntp_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool ntp_set_addr(char *hostname, sendip_data *pack);
bool ntp_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int ntp_num_opts(void);
sendip_option *ntp_get_opts(void);
char ntp_get_optchar(void);

/* BGP */
sendip_data *bgp_initialize(void);
bool bgp_do_opt(const char *optstring, char *optarg, sendip_data *pack);
bool bgp_set_addr(char *hostname, sendip_data *pack);
bool bgp_finalize(char *hdrs, sendip_data *headers[], sendip_data *data, sendip_data *pack);
int bgp_num_opts(void);
sendip_option *bgp_get_opts(void);
char bgp_get_optchar(void);

/* Array of available modules */
extern const module_entry available_modules[];
extern const int num_modules;

#endif /* _SENDIP_MODULES_H */
