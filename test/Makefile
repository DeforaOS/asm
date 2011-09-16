TARGETS	= amd64.o arm.o dalvik.o i386.o i386_real.o i486.o i586.o i686.o mips.o java.o sparc.o sparc64.o
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
AS	= ../src/asm-static
ASFLAGS	?=
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install


all: $(TARGETS)

amd64.o_OBJS = amd64.o
amd64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a amd64

arm.o_OBJS = arm.o
arm.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a arm

dalvik.o_OBJS = dalvik.o
dalvik.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a dalvik -f flat

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

mips.o_OBJS = mips.o
mips.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a mips

java.o_OBJS = java.o
java.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a java -f flat

sparc.o_OBJS = sparc.o
sparc.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc

sparc64.o_OBJS = sparc64.o
sparc64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc64

amd64.o: amd64.S ../src/asm
	$(AS) $(amd64.o_ASFLAGS) -o amd64.o amd64.S

arm.o: arm.S ../src/asm
	$(AS) $(arm.o_ASFLAGS) -o arm.o arm.S

dalvik.o: dalvik.S ../src/asm
	$(AS) $(dalvik.o_ASFLAGS) -o dalvik.o dalvik.S

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

mips.o: mips.S ../src/asm
	$(AS) $(mips.o_ASFLAGS) -o mips.o mips.S

java.o: java.S ../src/asm
	$(AS) $(java.o_ASFLAGS) -o java.o java.S

sparc.o: sparc.S ../src/asm
	$(AS) $(sparc.o_ASFLAGS) -o sparc.o sparc.S

sparc64.o: sparc64.S ../src/asm
	$(AS) $(sparc64.o_ASFLAGS) -o sparc64.o sparc64.S

clean:
	$(RM) -- $(amd64.o_OBJS) $(arm.o_OBJS) $(dalvik.o_OBJS) $(i386.o_OBJS) $(i386_real.o_OBJS) $(i486.o_OBJS) $(i586.o_OBJS) $(i686.o_OBJS) $(mips.o_OBJS) $(java.o_OBJS) $(sparc.o_OBJS) $(sparc64.o_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
