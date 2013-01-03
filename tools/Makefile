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

libasm_OBJS = arch.o arch/amd64.o arch/arm.o arch/armeb.o arch/armel.o arch/dalvik.o arch/i386.o arch/i386_real.o arch/i486.o arch/i586.o arch/i686.o arch/java.o arch/mips.o arch/mipseb.o arch/mipsel.o arch/sparc.o arch/sparc64.o arch/yasep.o arch/yasep16.o arch/yasep32.o format.o format/dex.o format/elf.o format/flat.o format/java.o format/pe.o
libasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags cpp`
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

arch/amd64.o: arch/amd64.c
	$(CC) $(libasm_CFLAGS) -o arch/amd64.o -c arch/amd64.c

arch/arm.o: arch/arm.c
	$(CC) $(libasm_CFLAGS) -o arch/arm.o -c arch/arm.c

arch/armeb.o: arch/armeb.c
	$(CC) $(libasm_CFLAGS) -o arch/armeb.o -c arch/armeb.c

arch/armel.o: arch/armel.c
	$(CC) $(libasm_CFLAGS) -o arch/armel.o -c arch/armel.c

arch/dalvik.o: arch/dalvik.c
	$(CC) $(libasm_CFLAGS) -o arch/dalvik.o -c arch/dalvik.c

arch/i386.o: arch/i386.c
	$(CC) $(libasm_CFLAGS) -o arch/i386.o -c arch/i386.c

arch/i386_real.o: arch/i386_real.c
	$(CC) $(libasm_CFLAGS) -o arch/i386_real.o -c arch/i386_real.c

arch/i486.o: arch/i486.c
	$(CC) $(libasm_CFLAGS) -o arch/i486.o -c arch/i486.c

arch/i586.o: arch/i586.c
	$(CC) $(libasm_CFLAGS) -o arch/i586.o -c arch/i586.c

arch/i686.o: arch/i686.c
	$(CC) $(libasm_CFLAGS) -o arch/i686.o -c arch/i686.c

arch/java.o: arch/java.c
	$(CC) $(libasm_CFLAGS) -o arch/java.o -c arch/java.c

arch/mips.o: arch/mips.c
	$(CC) $(libasm_CFLAGS) -o arch/mips.o -c arch/mips.c

arch/mipseb.o: arch/mipseb.c
	$(CC) $(libasm_CFLAGS) -o arch/mipseb.o -c arch/mipseb.c

arch/mipsel.o: arch/mipsel.c
	$(CC) $(libasm_CFLAGS) -o arch/mipsel.o -c arch/mipsel.c

arch/sparc.o: arch/sparc.c
	$(CC) $(libasm_CFLAGS) -o arch/sparc.o -c arch/sparc.c

arch/sparc64.o: arch/sparc64.c
	$(CC) $(libasm_CFLAGS) -o arch/sparc64.o -c arch/sparc64.c

arch/yasep.o: arch/yasep.c
	$(CC) $(libasm_CFLAGS) -o arch/yasep.o -c arch/yasep.c

arch/yasep16.o: arch/yasep16.c
	$(CC) $(libasm_CFLAGS) -o arch/yasep16.o -c arch/yasep16.c

arch/yasep32.o: arch/yasep32.c
	$(CC) $(libasm_CFLAGS) -o arch/yasep32.o -c arch/yasep32.c

format.o: format.c ../src/format.c
	$(CC) $(libasm_CFLAGS) -c format.c

format/dex.o: format/dex.c
	$(CC) $(libasm_CFLAGS) -o format/dex.o -c format/dex.c

format/elf.o: format/elf.c
	$(CC) $(libasm_CFLAGS) -o format/elf.o -c format/elf.c

format/flat.o: format/flat.c
	$(CC) $(libasm_CFLAGS) -o format/flat.o -c format/flat.c

format/java.o: format/java.c
	$(CC) $(libasm_CFLAGS) -o format/java.o -c format/java.c

format/pe.o: format/pe.c
	$(CC) $(libasm_CFLAGS) -o format/pe.o -c format/pe.c

clean:
	$(RM) -- $(libasm_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
