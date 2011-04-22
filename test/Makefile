TARGETS	= i386.o i386_real.o i486.o i586.o i686.o sparc.o sparc64.o
PREFIX	= /usr/local
DESTDIR	= 
BINDIR	= $(PREFIX)/bin
AS	= ../src/asm
ASFLAGS	= 
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

i386.o_OBJS = i386.o
i386.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i386

i386_real.o_OBJS = i386_real.o
i386_real.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i386_real -f flat

i486.o_OBJS = i486.o
i486.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i486

i586.o_OBJS = i586.o
i586.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i586

i686.o_OBJS = i686.o
i686.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i686

sparc.o_OBJS = sparc.o
sparc.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc

sparc64.o_OBJS = sparc64.o
sparc64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc64

i386.o: i386.S ../src/asm
	$(AS) $(i386.o_ASFLAGS) -o i386.o i386.S

i386_real.o: i386_real.S ../src/asm
	$(AS) $(i386_real.o_ASFLAGS) -o i386_real.o i386_real.S

i486.o: i486.S ../src/asm
	$(AS) $(i486.o_ASFLAGS) -o i486.o i486.S

i586.o: i586.S ../src/asm
	$(AS) $(i586.o_ASFLAGS) -o i586.o i586.S

i686.o: i686.S ../src/asm
	$(AS) $(i686.o_ASFLAGS) -o i686.o i686.S

sparc.o: sparc.S ../src/asm
	$(AS) $(sparc.o_ASFLAGS) -o sparc.o sparc.S

sparc64.o: sparc64.S ../src/asm
	$(AS) $(sparc64.o_ASFLAGS) -o sparc64.o sparc64.S

clean:
	$(RM) -- $(i386.o_OBJS) $(i386_real.o_OBJS) $(i486.o_OBJS) $(i586.o_OBJS) $(i686.o_OBJS) $(sparc.o_OBJS) $(sparc64.o_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
