TARGETS	= libasm.a libasm.so.0.0 libasm.so.0 libasm.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	?= cc
CPPFLAGSF= -I ../include -DSTANDALONE
CPPFLAGS?=
CFLAGSF	= -W -fPIC
CFLAGS	= -Wall -g -O2
AR	?= ar
RANLIB	?= ranlib
CCSHARED?= $(CC) -shared
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install


all: $(TARGETS)

libasm_OBJS = arch.o arch/arm.o arch/armeb.o arch/armel.o arch/i386.o arch/i486.o arch/i586.o arch/i686.o arch/mipseb.o arch/mipsel.o format.o
libasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
libasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) ../src/asm.o ../src/code.o ../src/parser.o ../src/token.o `pkg-config --libs cpp`

libasm.a: $(libasm_OBJS)
	$(AR) -rc libasm.a $(libasm_OBJS)
	$(RANLIB) libasm.a

libasm.so.0.0 libasm.so.0 libasm.so: $(libasm_OBJS)
	$(CCSHARED) -o libasm.so.0.0 -Wl,-soname,libasm.so.0 $(libasm_OBJS) $(libasm_LDFLAGS)
	$(LN) -s -- libasm.so.0.0 libasm.so.0
	$(LN) -s -- libasm.so.0.0 libasm.so

arch.o: arch.c ../src/arch.c
	$(CC) $(libasm_CFLAGS) -c arch.c

arch/arm.o: arch/arm.c
	$(CC) $(libasm_CFLAGS) -o arch/arm.o -c arch/arm.c

arch/armeb.o: arch/armeb.c
	$(CC) $(libasm_CFLAGS) -o arch/armeb.o -c arch/armeb.c

arch/armel.o: arch/armel.c
	$(CC) $(libasm_CFLAGS) -o arch/armel.o -c arch/armel.c

arch/i386.o: arch/i386.c
	$(CC) $(libasm_CFLAGS) -o arch/i386.o -c arch/i386.c

arch/i486.o: arch/i486.c
	$(CC) $(libasm_CFLAGS) -o arch/i486.o -c arch/i486.c

arch/i586.o: arch/i586.c
	$(CC) $(libasm_CFLAGS) -o arch/i586.o -c arch/i586.c

arch/i686.o: arch/i686.c
	$(CC) $(libasm_CFLAGS) -o arch/i686.o -c arch/i686.c

arch/mipseb.o: arch/mipseb.c
	$(CC) $(libasm_CFLAGS) -o arch/mipseb.o -c arch/mipseb.c

arch/mipsel.o: arch/mipsel.c
	$(CC) $(libasm_CFLAGS) -o arch/mipsel.o -c arch/mipsel.c

format.o: format.c ../src/format.c
	$(CC) $(libasm_CFLAGS) -c format.c

clean:
	$(RM) -- $(libasm_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
