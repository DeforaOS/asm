TARGETS	= $(OBJDIR)libAsm.a $(OBJDIR)libAsm.so.0.0 $(OBJDIR)libAsm.so.0 $(OBJDIR)libAsm.so $(OBJDIR)asm-static $(OBJDIR)deasm-static
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include -DSTANDALONE
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags cpp`
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs cpp`
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

libAsm_OBJS = $(OBJDIR)arch.o $(OBJDIR)arch/amd64.o $(OBJDIR)arch/arm.o $(OBJDIR)arch/armeb.o $(OBJDIR)arch/armel.o $(OBJDIR)arch/dalvik.o $(OBJDIR)arch/i386.o $(OBJDIR)arch/i386_real.o $(OBJDIR)arch/i486.o $(OBJDIR)arch/i586.o $(OBJDIR)arch/i686.o $(OBJDIR)arch/java.o $(OBJDIR)arch/mips.o $(OBJDIR)arch/mipseb.o $(OBJDIR)arch/mipsel.o $(OBJDIR)arch/sparc.o $(OBJDIR)arch/sparc64.o $(OBJDIR)arch/template.o $(OBJDIR)arch/yasep.o $(OBJDIR)arch/yasep16.o $(OBJDIR)arch/yasep32.o $(OBJDIR)format.o $(OBJDIR)format/dex.o $(OBJDIR)format/elf.o $(OBJDIR)format/elf/elf32.o $(OBJDIR)format/elf/elf64.o $(OBJDIR)format/flat.o $(OBJDIR)format/java.o $(OBJDIR)format/pe.o $(OBJDIR)../src/asm.o $(OBJDIR)../src/code.o $(OBJDIR)../src/parser.o $(OBJDIR)../src/token.o
libAsm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) -fPIC
libAsm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)libAsm.a: $(libAsm_OBJS)
	$(AR) -rc $(OBJDIR)libAsm.a $(libAsm_OBJS)
	$(RANLIB) $(OBJDIR)libAsm.a

$(OBJDIR)libAsm.so.0.0: $(libAsm_OBJS)
	$(CCSHARED) -o $(OBJDIR)libAsm.so.0.0 $(libAsm_OBJS) $(libAsm_LDFLAGS)

$(OBJDIR)libAsm.so.0: $(OBJDIR)libAsm.so.0.0
	$(LN) -s -- libAsm.so.0.0 $(OBJDIR)libAsm.so.0

$(OBJDIR)libAsm.so: $(OBJDIR)libAsm.so.0.0
	$(LN) -s -- libAsm.so.0.0 $(OBJDIR)libAsm.so
asm-static_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
asm-static_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) $(OBJDIR)../src/main.o $(OBJDIR)libAsm.a

$(OBJDIR)asm-static: $(asm-static_OBJS) $(OBJDIR)../src/main.o $(OBJDIR)libAsm.a
	$(CC) -o $(OBJDIR)asm-static $(asm-static_OBJS) $(asm-static_LDFLAGS)
deasm-static_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
deasm-static_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) $(OBJDIR)../src/deasm.o $(OBJDIR)libAsm.a

$(OBJDIR)deasm-static: $(deasm-static_OBJS) $(OBJDIR)../src/deasm.o $(OBJDIR)libAsm.a
	$(CC) -o $(OBJDIR)deasm-static $(deasm-static_OBJS) $(deasm-static_LDFLAGS)

$(OBJDIR)arch.o: arch.c ../src/arch.c
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch.o -c arch.c

$(OBJDIR)arch/amd64.o: arch/amd64.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/amd64.o -c arch/amd64.c

$(OBJDIR)arch/arm.o: arch/arm.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/arm.o -c arch/arm.c

$(OBJDIR)arch/armeb.o: arch/armeb.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/armeb.o -c arch/armeb.c

$(OBJDIR)arch/armel.o: arch/armel.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/armel.o -c arch/armel.c

$(OBJDIR)arch/dalvik.o: arch/dalvik.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/dalvik.o -c arch/dalvik.c

$(OBJDIR)arch/i386.o: arch/i386.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/i386.o -c arch/i386.c

$(OBJDIR)arch/i386_real.o: arch/i386_real.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/i386_real.o -c arch/i386_real.c

$(OBJDIR)arch/i486.o: arch/i486.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/i486.o -c arch/i486.c

$(OBJDIR)arch/i586.o: arch/i586.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/i586.o -c arch/i586.c

$(OBJDIR)arch/i686.o: arch/i686.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/i686.o -c arch/i686.c

$(OBJDIR)arch/java.o: arch/java.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/java.o -c arch/java.c

$(OBJDIR)arch/mips.o: arch/mips.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/mips.o -c arch/mips.c

$(OBJDIR)arch/mipseb.o: arch/mipseb.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/mipseb.o -c arch/mipseb.c

$(OBJDIR)arch/mipsel.o: arch/mipsel.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/mipsel.o -c arch/mipsel.c

$(OBJDIR)arch/sparc.o: arch/sparc.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/sparc.o -c arch/sparc.c

$(OBJDIR)arch/sparc64.o: arch/sparc64.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/sparc64.o -c arch/sparc64.c

$(OBJDIR)arch/template.o: arch/template.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/template.o -c arch/template.c

$(OBJDIR)arch/yasep.o: arch/yasep.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/yasep.o -c arch/yasep.c

$(OBJDIR)arch/yasep16.o: arch/yasep16.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/yasep16.o -c arch/yasep16.c

$(OBJDIR)arch/yasep32.o: arch/yasep32.c
	@[ -d "$(OBJDIR)arch" ] || $(MKDIR) -- "$(OBJDIR)arch"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)arch/yasep32.o -c arch/yasep32.c

$(OBJDIR)format.o: format.c ../src/format.c
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format.o -c format.c

$(OBJDIR)format/dex.o: format/dex.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/dex.o -c format/dex.c

$(OBJDIR)format/elf.o: format/elf.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/elf.o -c format/elf.c

$(OBJDIR)format/elf/elf32.o: format/elf/elf32.c
	@[ -d "$(OBJDIR)format/elf" ] || $(MKDIR) -- "$(OBJDIR)format/elf"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/elf/elf32.o -c format/elf/elf32.c

$(OBJDIR)format/elf/elf64.o: format/elf/elf64.c
	@[ -d "$(OBJDIR)format/elf" ] || $(MKDIR) -- "$(OBJDIR)format/elf"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/elf/elf64.o -c format/elf/elf64.c

$(OBJDIR)format/flat.o: format/flat.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/flat.o -c format/flat.c

$(OBJDIR)format/java.o: format/java.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/java.o -c format/java.c

$(OBJDIR)format/pe.o: format/pe.c
	@[ -d "$(OBJDIR)format" ] || $(MKDIR) -- "$(OBJDIR)format"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)format/pe.o -c format/pe.c

$(OBJDIR)../src/asm.o: ../src/asm.c
	@[ -d "$(OBJDIR)../src" ] || $(MKDIR) -- "$(OBJDIR)../src"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)../src/asm.o -c ../src/asm.c

$(OBJDIR)../src/code.o: ../src/code.c
	@[ -d "$(OBJDIR)../src" ] || $(MKDIR) -- "$(OBJDIR)../src"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)../src/code.o -c ../src/code.c

$(OBJDIR)../src/parser.o: ../src/parser.c
	@[ -d "$(OBJDIR)../src" ] || $(MKDIR) -- "$(OBJDIR)../src"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)../src/parser.o -c ../src/parser.c

$(OBJDIR)../src/token.o: ../src/token.c
	@[ -d "$(OBJDIR)../src" ] || $(MKDIR) -- "$(OBJDIR)../src"
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)../src/token.o -c ../src/token.c

clean:
	$(RM) -- $(libAsm_OBJS) $(asm-static_OBJS) $(deasm-static_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
