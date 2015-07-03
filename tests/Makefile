TARGETS	= $(OBJDIR)amd64.o $(OBJDIR)arm.o $(OBJDIR)armeb.o $(OBJDIR)armel.o $(OBJDIR)dalvik.o $(OBJDIR)i386.o $(OBJDIR)i386_real.o $(OBJDIR)i486.o $(OBJDIR)i586.o $(OBJDIR)i686.o $(OBJDIR)mips.o $(OBJDIR)mipseb.o $(OBJDIR)mipsel.o $(OBJDIR)java.o $(OBJDIR)pylint.log $(OBJDIR)sparc.o $(OBJDIR)sparc64.o $(OBJDIR)template.o $(OBJDIR)tests.log $(OBJDIR)yasep.o $(OBJDIR)yasep16.o $(OBJDIR)yasep32.o
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
AS	= $(OBJDIR)../tools/asm-static
ASFLAGS	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

amd64.o_OBJS = $(OBJDIR)amd64.o
amd64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a amd64

arm.o_OBJS = $(OBJDIR)arm.o
arm.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a arm

armeb.o_OBJS = $(OBJDIR)armeb.o
armeb.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a armeb

armel.o_OBJS = $(OBJDIR)armel.o
armel.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a armel

dalvik.o_OBJS = $(OBJDIR)dalvik.o
dalvik.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a dalvik -f flat

i386.o_OBJS = $(OBJDIR)i386.o
i386.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i386

i386_real.o_OBJS = $(OBJDIR)i386_real.o
i386_real.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i386_real -f flat

i486.o_OBJS = $(OBJDIR)i486.o
i486.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i486

i586.o_OBJS = $(OBJDIR)i586.o
i586.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i586

i686.o_OBJS = $(OBJDIR)i686.o
i686.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i686

mips.o_OBJS = $(OBJDIR)mips.o
mips.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a mips

mipseb.o_OBJS = $(OBJDIR)mipseb.o
mipseb.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a mipseb

mipsel.o_OBJS = $(OBJDIR)mipsel.o
mipsel.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a mipsel

java.o_OBJS = $(OBJDIR)java.o
java.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a java -f flat

$(OBJDIR)pylint.log: pylint.sh
	./pylint.sh -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"

sparc.o_OBJS = $(OBJDIR)sparc.o
sparc.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc

sparc64.o_OBJS = $(OBJDIR)sparc64.o
sparc64.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc64

template.o_OBJS = $(OBJDIR)template.o
template.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a template -f flat

$(OBJDIR)tests.log: $(OBJDIR)amd64.o $(OBJDIR)arm.o $(OBJDIR)armeb.o $(OBJDIR)armel.o $(OBJDIR)i386.o $(OBJDIR)i386_real.o $(OBJDIR)i486.o $(OBJDIR)i586.o $(OBJDIR)i686.o python.sh $(OBJDIR)sparc.o $(OBJDIR)sparc64.o tests.sh $(OBJDIR)yasep.o $(OBJDIR)yasep16.o $(OBJDIR)yasep32.o
	./tests.sh -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

yasep.o_OBJS = $(OBJDIR)yasep.o
yasep.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a yasep -f flat

yasep16.o_OBJS = $(OBJDIR)yasep16.o
yasep16.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a yasep16 -f flat

yasep32.o_OBJS = $(OBJDIR)yasep32.o
yasep32.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a yasep32 -f flat

$(OBJDIR)amd64.o: amd64.asm
	$(AS) $(amd64.o_ASFLAGS) -o $(OBJDIR)amd64.o amd64.asm

$(OBJDIR)arm.o: arm.asm
	$(AS) $(arm.o_ASFLAGS) -o $(OBJDIR)arm.o arm.asm

$(OBJDIR)armeb.o: armeb.asm
	$(AS) $(armeb.o_ASFLAGS) -o $(OBJDIR)armeb.o armeb.asm

$(OBJDIR)armel.o: armel.asm
	$(AS) $(armel.o_ASFLAGS) -o $(OBJDIR)armel.o armel.asm

$(OBJDIR)dalvik.o: dalvik.asm
	$(AS) $(dalvik.o_ASFLAGS) -o $(OBJDIR)dalvik.o dalvik.asm

$(OBJDIR)i386.o: i386.asm
	$(AS) $(i386.o_ASFLAGS) -o $(OBJDIR)i386.o i386.asm

$(OBJDIR)i386_real.o: i386_real.asm
	$(AS) $(i386_real.o_ASFLAGS) -o $(OBJDIR)i386_real.o i386_real.asm

$(OBJDIR)i486.o: i486.asm
	$(AS) $(i486.o_ASFLAGS) -o $(OBJDIR)i486.o i486.asm

$(OBJDIR)i586.o: i586.asm
	$(AS) $(i586.o_ASFLAGS) -o $(OBJDIR)i586.o i586.asm

$(OBJDIR)i686.o: i686.asm
	$(AS) $(i686.o_ASFLAGS) -o $(OBJDIR)i686.o i686.asm

$(OBJDIR)mips.o: mips.asm
	$(AS) $(mips.o_ASFLAGS) -o $(OBJDIR)mips.o mips.asm

$(OBJDIR)mipseb.o: mipseb.asm
	$(AS) $(mipseb.o_ASFLAGS) -o $(OBJDIR)mipseb.o mipseb.asm

$(OBJDIR)mipsel.o: mipsel.asm
	$(AS) $(mipsel.o_ASFLAGS) -o $(OBJDIR)mipsel.o mipsel.asm

$(OBJDIR)java.o: java.asm
	$(AS) $(java.o_ASFLAGS) -o $(OBJDIR)java.o java.asm

$(OBJDIR)sparc.o: sparc.asm
	$(AS) $(sparc.o_ASFLAGS) -o $(OBJDIR)sparc.o sparc.asm

$(OBJDIR)sparc64.o: sparc64.asm
	$(AS) $(sparc64.o_ASFLAGS) -o $(OBJDIR)sparc64.o sparc64.asm

$(OBJDIR)template.o: template.asm
	$(AS) $(template.o_ASFLAGS) -o $(OBJDIR)template.o template.asm

$(OBJDIR)yasep.o: yasep.asm
	$(AS) $(yasep.o_ASFLAGS) -o $(OBJDIR)yasep.o yasep.asm

$(OBJDIR)yasep16.o: yasep16.asm
	$(AS) $(yasep16.o_ASFLAGS) -o $(OBJDIR)yasep16.o yasep16.asm

$(OBJDIR)yasep32.o: yasep32.asm
	$(AS) $(yasep32.o_ASFLAGS) -o $(OBJDIR)yasep32.o yasep32.asm

clean:
	$(RM) -- $(amd64.o_OBJS) $(arm.o_OBJS) $(armeb.o_OBJS) $(armel.o_OBJS) $(dalvik.o_OBJS) $(i386.o_OBJS) $(i386_real.o_OBJS) $(i486.o_OBJS) $(i586.o_OBJS) $(i686.o_OBJS) $(mips.o_OBJS) $(mipseb.o_OBJS) $(mipsel.o_OBJS) $(java.o_OBJS) $(pylint.log_OBJS) $(sparc.o_OBJS) $(sparc64.o_OBJS) $(template.o_OBJS) $(tests.log_OBJS) $(yasep.o_OBJS) $(yasep16.o_OBJS) $(yasep32.o_OBJS)
	./pylint.sh -c -P "$(PREFIX)" -- "pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "tests.log"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
