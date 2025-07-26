#configureable stuff
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/share/man/man1
LIBDIR ?= $(PREFIX)/lib/sendip
#For most systems, this works
INSTALL ?= install
#For Solaris, you may need
#INSTALL=/usr/ucb/install

CFLAGS=	-fPIC -fsigned-char -pipe -Wall -Wpointer-arith -Wwrite-strings \
			-Wstrict-prototypes -Wnested-externs -Winline -Werror -g -Wcast-align
#-Wcast-align causes problems on solaris, but not serious ones
LDFLAGS=	-g -lm
#LDFLAGS_SOLARIS= -g -lsocket -lnsl -lm
LDFLAGS_SOLARIS= -g
LIBS_SOLARIS= -lsocket -lnsl -lm
LDFLAGS_LINUX= -g
LIBS_LINUX= -lm
LIBCFLAGS= -shared
CC=	gcc

PROGS= sendip
BASEPROTOS_OBJ= ipv4.o ipv6.o
IPPROTOS_OBJ= icmp.o tcp.o udp.o
UDPPROTOS_OBJ= rip.o ripng.o ntp.o
TCPPROTOS_OBJ= bgp.o
PROTOOBJS= $(BASEPROTOS_OBJ) $(IPPROTOS_OBJ) $(UDPPROTOS_OBJ) $(TCPPROTOS_OBJ)
GLOBALOBJS= csum.o compact.o modules.o

all:	$(GLOBALOBJS) sendip sendip.1 sendip.spec

#there has to be a nice way to do this
sendip:	sendip.o gnugetopt.o gnugetopt1.o $(GLOBALOBJS) $(PROTOOBJS)
	sh -c "if [ `uname` = Linux ] ; then \
echo $(CC) -o $@ $(LDFLAGS_LINUX) $(CFLAGS) $+ ; \
$(CC) -o $@ $(LDFLAGS_LINUX) $(CFLAGS) $+ $(LIBS_LINUX) ; \
elif [ `uname` = SunOS ] ; then \
echo $(CC) -o $@ $(LDFLAGS_SOLARIS) $(CFLAGS) $+ ;\
$(CC) -o $@ $(LDFLAGS_SOLARIS) $(CFLAGS) $+ $(LIBS_SOLARIS) ;\
else \
echo $(CC) -o $@ $(LDFLAGS) $(CFLAGS) $+ ; \
$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $+ ; \
fi"

sendip.1:	./help2man $(PROGS) $(PROTOS) VERSION
			./help2man -n "Send arbitrary IP packets" -N >sendip.1

sendip.spec:	sendip.spec.in VERSION
			echo -n '%define ver ' >sendip.spec
			cat VERSION >>sendip.spec
			cat sendip.spec.in >>sendip.spec

# Removed shared object rule as we're now building static objects

.PHONY:	clean install

clean:
			rm -f *.o *~ $(PROGS) core gmon.out

veryclean:
			make clean
			rm -f sendip.spec sendip.1

install:		all
			[ -d $(BINDIR) ] || mkdir -p $(BINDIR)
			[ -d $(MANDIR) ] || mkdir -p $(MANDIR)
			$(INSTALL) -m 755 $(PROGS) $(BINDIR)
			$(INSTALL) -m 644 sendip.1 $(MANDIR)
