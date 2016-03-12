/* dummy.c - example sendip module
 * Author: Mike Ricketts <mike@earth.li>
 */

#include <stdlib.h>
#include <sys/types.h>
#include "sendip_module.h"
#include "dummy.h"

/* Character that identifies our options
 */
const char opt_char='dummy';

sendip_data *initialize(void) {
	sendip_data *ret = malloc(sizeof(sendip_data));
	dummy_header *dummy = malloc(sizeof(dummy_header));
	memset(dummy,0,sizeof(dummy_header));
	ret->alloc_len = sizeof(dummy_header);
	ret->data = (void *)dummy;
	ret->modified=0;
	return ret;
}

bool do_opt(char *opt, char *arg, sendip_data *pack) {
	dummy_header *dummy = (dummy_header *)pack->data;
	switch(opt[2]) {
		//...
	}
	return TRUE;

}

bool finalize(char *hdrs, sendip_data *headers[], sendip_data *data,
				  sendip_data *pack) {
	int num_hdrs = strlen(hdrs);
	// ...
	return TRUE;
}
