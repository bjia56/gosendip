#include "modules.h"

/* Module registration array */
const module_entry available_modules[] = {
    {
        .name = "ipv4",
        .optchar = 'i',
        .initialize = ipv4_initialize,
        .do_opt = ipv4_do_opt,
        .set_addr = ipv4_set_addr,
        .finalize = ipv4_finalize,
        .num_opts = ipv4_num_opts,
        .get_opts = ipv4_get_opts,
        .get_optchar = ipv4_get_optchar
    },
    {
        .name = "ipv6",
        .optchar = 'g',
        .initialize = ipv6_initialize,
        .do_opt = ipv6_do_opt,
        .set_addr = ipv6_set_addr,
        .finalize = ipv6_finalize,
        .num_opts = ipv6_num_opts,
        .get_opts = ipv6_get_opts,
        .get_optchar = ipv6_get_optchar
    },
    {
        .name = "icmp",
        .optchar = 'c',
        .initialize = icmp_initialize,
        .do_opt = icmp_do_opt,
        .set_addr = icmp_set_addr,
        .finalize = icmp_finalize,
        .num_opts = icmp_num_opts,
        .get_opts = icmp_get_opts,
        .get_optchar = icmp_get_optchar
    },
    {
        .name = "tcp",
        .optchar = 't',
        .initialize = tcp_initialize,
        .do_opt = tcp_do_opt,
        .set_addr = tcp_set_addr,
        .finalize = tcp_finalize,
        .num_opts = tcp_num_opts,
        .get_opts = tcp_get_opts,
        .get_optchar = tcp_get_optchar
    },
    {
        .name = "udp",
        .optchar = 'u',
        .initialize = udp_initialize,
        .do_opt = udp_do_opt,
        .set_addr = udp_set_addr,
        .finalize = udp_finalize,
        .num_opts = udp_num_opts,
        .get_opts = udp_get_opts,
        .get_optchar = udp_get_optchar
    },
    {
        .name = "rip",
        .optchar = 'r',
        .initialize = rip_initialize,
        .do_opt = rip_do_opt,
        .set_addr = rip_set_addr,
        .finalize = rip_finalize,
        .num_opts = rip_num_opts,
        .get_opts = rip_get_opts,
        .get_optchar = rip_get_optchar
    },
    {
        .name = "ripng",
        .optchar = 'n',
        .initialize = ripng_initialize,
        .do_opt = ripng_do_opt,
        .set_addr = ripng_set_addr,
        .finalize = ripng_finalize,
        .num_opts = ripng_num_opts,
        .get_opts = ripng_get_opts,
        .get_optchar = ripng_get_optchar
    },
    {
        .name = "ntp",
        .optchar = 'p',
        .initialize = ntp_initialize,
        .do_opt = ntp_do_opt,
        .set_addr = ntp_set_addr,
        .finalize = ntp_finalize,
        .num_opts = ntp_num_opts,
        .get_opts = ntp_get_opts,
        .get_optchar = ntp_get_optchar
    },
    {
        .name = "bgp",
        .optchar = 'b',
        .initialize = bgp_initialize,
        .do_opt = bgp_do_opt,
        .set_addr = bgp_set_addr,
        .finalize = bgp_finalize,
        .num_opts = bgp_num_opts,
        .get_opts = bgp_get_opts,
        .get_optchar = bgp_get_optchar
    }
};

const int num_modules = sizeof(available_modules) / sizeof(module_entry);
