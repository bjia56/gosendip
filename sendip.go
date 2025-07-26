package gosendip

// #cgo CFLAGS: -fPIC -pipe -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -Wnested-externs -Winline -Wcast-align
// #include <sys/types.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>
//
// #include "./gnugetopt.h"
// #include "./modules.h"
// #include "./types.h"
// #include "./sendip_module.h"
//
// #include "./icmp.h"
// #include "./ipv4.h"
// #include "./ipv6.h"
// #include "./ntp.h"
// #include "./rip.h"
// #include "./ripng.h"
// #include "./tcp.h"
// #include "./udp.h"
import "C"
import "unsafe"

func SendIP(args []string) (int, error) {
	cArgs := make([]*C.char, len(args)+1)
	for i, arg := range args {
		cArgs[i+1] = C.CString(arg)
	}
	cArgs[0] = C.CString("sendip")

	result := C.sendip_main(C.int(len(cArgs)), &cArgs[0])
	for _, arg := range cArgs {
		C.free(unsafe.Pointer(arg)) // Free each C string
	}

	return int(result), nil
}
