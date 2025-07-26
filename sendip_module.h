#ifndef _SENDIP_MODULE_H
#define _SENDIP_MODULE_H

#include <stdio.h>   // for fprintf

#include "types.h"

/* Options
 */
typedef struct {
	const char *optname;
	const bool arg;
	const char *description;
	const char *def;
} sendip_option;

/* Data
 */
typedef struct {
	void *data;
	int alloc_len;
	unsigned int modified;
} sendip_data;

int sendip_main(int argc, char *const argv[]);

#define usage_error(x) fprintf(stderr,x)

extern u_int16_t csum(u_int16_t *packet, int packlen);
extern int compact_string(char *data_out);

#endif  /* _SENDIP_MODULE_H */
