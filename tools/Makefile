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

libasm_OBJS = arch.o format.o
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

format.o: format.c ../src/format.c
	$(CC) $(libasm_CFLAGS) -c format.c

clean:
	$(RM) -- $(libasm_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
