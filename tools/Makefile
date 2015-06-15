TARGETS	= $(OBJDIR)libasm.a $(OBJDIR)libasm.so.0.0 $(OBJDIR)libasm.so.0 $(OBJDIR)libasm.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include -DSTANDALONE
CPPFLAGS=
CFLAGSF	= -W -fPIC
CFLAGS	= -Wall -g -O2
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

libasm_OBJS = $(OBJDIR)arch.o $(OBJDIR)arch/amd64.o $(OBJDIR)arch/arm.o $(OBJDIR)arch/armeb.o $(OBJDIR)arch/armel.o $(OBJDIR)arch/dalvik.o $(OBJDIR)arch/i386.o $(OBJDIR)arch/i386_real.o $(OBJDIR)arch/i486.o $(OBJDIR)arch/i586.o $(OBJDIR)arch/i686.o $(OBJDIR)arch/java.o $(OBJDIR)arch/mips.o $(OBJDIR)arch/mipseb.o $(OBJDIR)arch/mipsel.o $(OBJDIR)arch/sparc.o $(OBJDIR)arch/sparc64.o $(OBJDIR)arch/yasep.o $(OBJDIR)arch/yasep16.o $(OBJDIR)arch/yasep32.o $(OBJDIR)format.o $(OBJDIR)format/dex.o $(OBJDIR)format/elf.o $(OBJDIR)format/flat.o $(OBJDIR)format/java.o $(OBJDIR)format/pe.o
libasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags cpp`
libasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) ../src/asm.o ../src/code.o ../src/parser.o ../src/token.o `pkg-config --libs cpp`

$(OBJDIR)libasm.a: $(libasm_OBJS)
	$(AR) -rc $(OBJDIR)libasm.a $(libasm_OBJS)
	$(RANLIB) $(OBJDIR)libasm.a

$(OBJDIR)libasm.so.0.0: $(libasm_OBJS)
	$(CCSHARED) -o $(OBJDIR)libasm.so.0.0 $(libasm_OBJS) $(libasm_LDFLAGS)

$(OBJDIR)libasm.so.0: $(OBJDIR)libasm.so.0.0
	$(LN) -s -- libasm.so.0.0 $(OBJDIR)libasm.so.0

$(OBJDIR)libasm.so: $(OBJDIR)libasm.so.0.0
	$(LN) -s -- libasm.so.0.0 $(OBJDIR)libasm.so

$(OBJDIR)arch.o: arch.c ../src/arch.c
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch.o -c arch.c

$(OBJDIR)arch/amd64.o: arch/amd64.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/amd64.o -c arch/amd64.c

$(OBJDIR)arch/arm.o: arch/arm.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/arm.o -c arch/arm.c

$(OBJDIR)arch/armeb.o: arch/armeb.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/armeb.o -c arch/armeb.c

$(OBJDIR)arch/armel.o: arch/armel.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/armel.o -c arch/armel.c

$(OBJDIR)arch/dalvik.o: arch/dalvik.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/dalvik.o -c arch/dalvik.c

$(OBJDIR)arch/i386.o: arch/i386.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/i386.o -c arch/i386.c

$(OBJDIR)arch/i386_real.o: arch/i386_real.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/i386_real.o -c arch/i386_real.c

$(OBJDIR)arch/i486.o: arch/i486.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/i486.o -c arch/i486.c

$(OBJDIR)arch/i586.o: arch/i586.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/i586.o -c arch/i586.c

$(OBJDIR)arch/i686.o: arch/i686.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/i686.o -c arch/i686.c

$(OBJDIR)arch/java.o: arch/java.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/java.o -c arch/java.c

$(OBJDIR)arch/mips.o: arch/mips.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/mips.o -c arch/mips.c

$(OBJDIR)arch/mipseb.o: arch/mipseb.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/mipseb.o -c arch/mipseb.c

$(OBJDIR)arch/mipsel.o: arch/mipsel.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/mipsel.o -c arch/mipsel.c

$(OBJDIR)arch/sparc.o: arch/sparc.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/sparc.o -c arch/sparc.c

$(OBJDIR)arch/sparc64.o: arch/sparc64.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/sparc64.o -c arch/sparc64.c

$(OBJDIR)arch/yasep.o: arch/yasep.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/yasep.o -c arch/yasep.c

$(OBJDIR)arch/yasep16.o: arch/yasep16.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/yasep16.o -c arch/yasep16.c

$(OBJDIR)arch/yasep32.o: arch/yasep32.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)arch/yasep32.o -c arch/yasep32.c

$(OBJDIR)format.o: format.c ../src/format.c
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)format.o -c format.c

$(OBJDIR)format/dex.o: format/dex.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)format/dex.o -c format/dex.c

$(OBJDIR)format/elf.o: format/elf.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)format/elf.o -c format/elf.c

$(OBJDIR)format/flat.o: format/flat.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)format/flat.o -c format/flat.c

$(OBJDIR)format/java.o: format/java.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)format/java.o -c format/java.c

$(OBJDIR)format/pe.o: format/pe.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)format/pe.o -c format/pe.c

clean:
	$(RM) -- $(libasm_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
