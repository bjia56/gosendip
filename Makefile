#configureable stuff
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/man/man1
LIBDIR=$(PREFIX)/lib/sendip

CFLAGS=	-pipe -Wall -Wpointer-arith -Wcast-align -Wwrite-strings \
			-Wstrict-prototypes -Wnested-externs -Winline -Werror -g -pg \
			-DSENDIP_LIBS=\"$(LIBDIR)\"
LDFLAGS=	-g -pg -rdynamic
LIBCFLAGS= -shared
CC=	gcc

PROGS= sendip
PROTOS= ipv4.so ipv6.so icmp.so tcp.so udp.so rip.so bgp.so #dns.so
GLOBALOBJS= csum.o

all:	$(GLOBALOBJS) sendip $(PROTOS) sendip.1 sendip.spec

#there has to be a nice way to do this
sendip:	sendip.o	getopt.o getopt1.o
	sh -c "if [ `uname` = Linux ] ; then \
$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $+ -ldl ; else \
$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $+ ; fi"

sendip.1:	./help2man $(PROGS) $(PROTOS) VERSION
			./help2man -n "Send arbitrary IP packets" -N >sendip.1

sendip.spec:	sendip.spec.in VERSION
			echo -n '%define ver ' >sendip.spec
			cat VERSION >>sendip.spec
			cat sendip.spec.in >>sendip.spec

%.so: %.c
			$(CC) -o $@ $(CFLAGS) $(LIBCFLAGS) $+ $(GLOBALOBJS)

.PHONY:	clean install

clean:
			rm -f *.o *~ *.so $(PROTOS) $(PROGS) core gmon.out

veryclean:
			make clean
			rm -f sendip.spec sendip.1

install:		all
			[ -d $(LIBDIR) ] || mkdir -p $(LIBDIR)
			install -m 755 $(PROGS) $(BINDIR)
			install -m 644 sendip.1 $(MANDIR)
			install -m 755 $(PROTOS) $(LIBDIR)
