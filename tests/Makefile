TARGETS	= amd64.o arm.o armeb.o armel.o dalvik.o i386.o i386_real.o i486.o i586.o i686.o mips.o mipseb.o mipsel.o java.o sparc.o sparc64.o tests.txt yasep.o yasep16.o yasep32.o
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
AS	= ../src/asm-static
ASFLAGS	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

amd64.o_OBJS = amd64.o
amd64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a amd64

arm.o_OBJS = arm.o
arm.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a arm

armeb.o_OBJS = armeb.o
armeb.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a armeb

armel.o_OBJS = armel.o
armel.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a armel

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

mipseb.o_OBJS = mipseb.o
mipseb.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a mipseb

mipsel.o_OBJS = mipsel.o
mipsel.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a mipsel

java.o_OBJS = java.o
java.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a java -f flat

sparc.o_OBJS = sparc.o
sparc.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc

sparc64.o_OBJS = sparc64.o
sparc64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc64

tests.txt: amd64.o arm.o armeb.o armel.o i386.o i386_real.o i486.o i586.o i686.o sparc.o sparc64.o yasep.o yasep16.o yasep32.o
	./tests.sh -P "$(PREFIX)" -- "tests.txt"

yasep.o_OBJS = yasep.o
yasep.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a yasep -f flat

yasep16.o_OBJS = yasep16.o
yasep16.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a yasep16 -f flat

yasep32.o_OBJS = yasep32.o
yasep32.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a yasep32 -f flat

amd64.o: amd64.asm ../src/asm
	$(AS) $(amd64.o_ASFLAGS) -o amd64.o amd64.asm

arm.o: arm.asm ../src/arch/arm.so ../src/asm
	$(AS) $(arm.o_ASFLAGS) -o arm.o arm.asm

armeb.o: armeb.asm ../src/arch/armeb.so arm.asm ../src/asm
	$(AS) $(armeb.o_ASFLAGS) -o armeb.o armeb.asm

armel.o: armel.asm ../src/arch/armel.so arm.asm ../src/asm
	$(AS) $(armel.o_ASFLAGS) -o armel.o armel.asm

dalvik.o: dalvik.asm ../src/asm
	$(AS) $(dalvik.o_ASFLAGS) -o dalvik.o dalvik.asm

i386.o: i386.asm ../src/asm
	$(AS) $(i386.o_ASFLAGS) -o i386.o i386.asm

i386_real.o: i386_real.asm ../src/asm
	$(AS) $(i386_real.o_ASFLAGS) -o i386_real.o i386_real.asm

i486.o: i486.asm ../src/asm
	$(AS) $(i486.o_ASFLAGS) -o i486.o i486.asm

i586.o: i586.asm ../src/asm
	$(AS) $(i586.o_ASFLAGS) -o i586.o i586.asm

i686.o: i686.asm ../src/asm
	$(AS) $(i686.o_ASFLAGS) -o i686.o i686.asm

mips.o: mips.asm ../src/asm
	$(AS) $(mips.o_ASFLAGS) -o mips.o mips.asm

mipseb.o: mipseb.asm mips.asm ../src/asm
	$(AS) $(mipseb.o_ASFLAGS) -o mipseb.o mipseb.asm

mipsel.o: mipsel.asm mips.asm ../src/asm
	$(AS) $(mipsel.o_ASFLAGS) -o mipsel.o mipsel.asm

java.o: java.asm ../src/asm
	$(AS) $(java.o_ASFLAGS) -o java.o java.asm

sparc.o: sparc.asm ../src/asm
	$(AS) $(sparc.o_ASFLAGS) -o sparc.o sparc.asm

sparc64.o: sparc64.asm ../src/asm
	$(AS) $(sparc64.o_ASFLAGS) -o sparc64.o sparc64.asm

yasep.o: yasep.asm ../src/asm
	$(AS) $(yasep.o_ASFLAGS) -o yasep.o yasep.asm

yasep16.o: yasep16.asm ../src/asm yasep.asm
	$(AS) $(yasep16.o_ASFLAGS) -o yasep16.o yasep16.asm

yasep32.o: yasep32.asm ../src/asm yasep.asm
	$(AS) $(yasep32.o_ASFLAGS) -o yasep32.o yasep32.asm

clean:
	$(RM) -- $(amd64.o_OBJS) $(arm.o_OBJS) $(armeb.o_OBJS) $(armel.o_OBJS) $(dalvik.o_OBJS) $(i386.o_OBJS) $(i386_real.o_OBJS) $(i486.o_OBJS) $(i586.o_OBJS) $(i686.o_OBJS) $(mips.o_OBJS) $(mipseb.o_OBJS) $(mipsel.o_OBJS) $(java.o_OBJS) $(sparc.o_OBJS) $(sparc64.o_OBJS) $(tests.txt_OBJS) $(yasep.o_OBJS) $(yasep16.o_OBJS) $(yasep32.o_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
