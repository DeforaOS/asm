TARGETS	= amd64.so arm.so armeb.so armel.so dalvik.so i386.so i386_real.so i486.so i586.so i686.so java.so mips.so mipseb.so mipsel.so sparc.so sparc64.so template.so yasep.so yasep16.so yasep32.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libSystem`
CFLAGS	= -Wall -g -O2 -fPIC -pedantic
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

amd64_OBJS = amd64.o
amd64_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
amd64_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

amd64.so: $(amd64_OBJS)
	$(CCSHARED) -o amd64.so $(amd64_OBJS) $(amd64_LDFLAGS)

arm_OBJS = arm.o
arm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
arm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

arm.so: $(arm_OBJS)
	$(CCSHARED) -o arm.so $(arm_OBJS) $(arm_LDFLAGS)

armeb_OBJS = armeb.o
armeb_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
armeb_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

armeb.so: $(armeb_OBJS)
	$(CCSHARED) -o armeb.so $(armeb_OBJS) $(armeb_LDFLAGS)

armel_OBJS = armel.o
armel_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
armel_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

armel.so: $(armel_OBJS)
	$(CCSHARED) -o armel.so $(armel_OBJS) $(armel_LDFLAGS)

dalvik_OBJS = dalvik.o
dalvik_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
dalvik_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

dalvik.so: $(dalvik_OBJS)
	$(CCSHARED) -o dalvik.so $(dalvik_OBJS) $(dalvik_LDFLAGS)

i386_OBJS = i386.o
i386_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i386.so: $(i386_OBJS)
	$(CCSHARED) -o i386.so $(i386_OBJS) $(i386_LDFLAGS)

i386_real_OBJS = i386_real.o
i386_real_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_real_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i386_real.so: $(i386_real_OBJS)
	$(CCSHARED) -o i386_real.so $(i386_real_OBJS) $(i386_real_LDFLAGS)

i486_OBJS = i486.o
i486_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i486_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i486.so: $(i486_OBJS)
	$(CCSHARED) -o i486.so $(i486_OBJS) $(i486_LDFLAGS)

i586_OBJS = i586.o
i586_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i586_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i586.so: $(i586_OBJS)
	$(CCSHARED) -o i586.so $(i586_OBJS) $(i586_LDFLAGS)

i686_OBJS = i686.o
i686_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i686_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i686.so: $(i686_OBJS)
	$(CCSHARED) -o i686.so $(i686_OBJS) $(i686_LDFLAGS)

java_OBJS = java.o
java_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
java_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

java.so: $(java_OBJS)
	$(CCSHARED) -o java.so $(java_OBJS) $(java_LDFLAGS)

mips_OBJS = mips.o
mips_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mips_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

mips.so: $(mips_OBJS)
	$(CCSHARED) -o mips.so $(mips_OBJS) $(mips_LDFLAGS)

mipseb_OBJS = mipseb.o
mipseb_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mipseb_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

mipseb.so: $(mipseb_OBJS)
	$(CCSHARED) -o mipseb.so $(mipseb_OBJS) $(mipseb_LDFLAGS)

mipsel_OBJS = mipsel.o
mipsel_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mipsel_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

mipsel.so: $(mipsel_OBJS)
	$(CCSHARED) -o mipsel.so $(mipsel_OBJS) $(mipsel_LDFLAGS)

sparc_OBJS = sparc.o
sparc_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

sparc.so: $(sparc_OBJS)
	$(CCSHARED) -o sparc.so $(sparc_OBJS) $(sparc_LDFLAGS)

sparc64_OBJS = sparc64.o
sparc64_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc64_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

sparc64.so: $(sparc64_OBJS)
	$(CCSHARED) -o sparc64.so $(sparc64_OBJS) $(sparc64_LDFLAGS)

template_OBJS = template.o
template_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
template_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

template.so: $(template_OBJS)
	$(CCSHARED) -o template.so $(template_OBJS) $(template_LDFLAGS)

yasep_OBJS = yasep.o
yasep_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
yasep_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

yasep.so: $(yasep_OBJS)
	$(CCSHARED) -o yasep.so $(yasep_OBJS) $(yasep_LDFLAGS)

yasep16_OBJS = yasep16.o
yasep16_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
yasep16_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

yasep16.so: $(yasep16_OBJS)
	$(CCSHARED) -o yasep16.so $(yasep16_OBJS) $(yasep16_LDFLAGS)

yasep32_OBJS = yasep32.o
yasep32_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
yasep32_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

yasep32.so: $(yasep32_OBJS)
	$(CCSHARED) -o yasep32.so $(yasep32_OBJS) $(yasep32_LDFLAGS)

amd64.o: amd64.c amd64.ins amd64.reg common.ins null.ins i386.h i386.ins i386.reg i486.ins i686.ins i686.reg
	$(CC) $(amd64_CFLAGS) -c amd64.c

arm.o: arm.c arm.h arm.ins arm.reg common.ins null.ins
	$(CC) $(arm_CFLAGS) -c arm.c

armeb.o: armeb.c arm.o
	$(CC) $(armeb_CFLAGS) -c armeb.c

armel.o: armel.c arm.o
	$(CC) $(armel_CFLAGS) -c armel.c

dalvik.o: dalvik.c common.ins null.ins dalvik.ins dalvik.reg
	$(CC) $(dalvik_CFLAGS) -c dalvik.c

i386.o: i386.c common.ins null.ins i386.h i386.ins i386.reg
	$(CC) $(i386_CFLAGS) -c i386.c

i386_real.o: i386_real.c common.ins null.ins i386.h i386.ins i386.reg
	$(CC) $(i386_real_CFLAGS) -c i386_real.c

i486.o: i486.c common.ins null.ins i386.h i386.ins i386.reg i486.ins
	$(CC) $(i486_CFLAGS) -c i486.c

i586.o: i586.c common.ins null.ins i386.h i386.ins i386.reg i486.ins i586.ins
	$(CC) $(i586_CFLAGS) -c i586.c

i686.o: i686.c common.ins null.ins i386.h i386.ins i386.reg i486.ins i686.ins i686.reg
	$(CC) $(i686_CFLAGS) -c i686.c

java.o: java.c common.ins null.ins
	$(CC) $(java_CFLAGS) -c java.c

mips.o: mips.c common.ins null.ins mips.h mips.ins mips.reg
	$(CC) $(mips_CFLAGS) -c mips.c

mipseb.o: mipseb.c mips.o
	$(CC) $(mipseb_CFLAGS) -c mipseb.c

mipsel.o: mipsel.c mips.o
	$(CC) $(mipsel_CFLAGS) -c mipsel.c

sparc.o: sparc.c common.ins null.ins sparc.h sparc.ins sparc.reg
	$(CC) $(sparc_CFLAGS) -c sparc.c

sparc64.o: sparc64.c common.ins null.ins sparc.h sparc.ins sparc.reg
	$(CC) $(sparc64_CFLAGS) -c sparc64.c

template.o: template.c common.ins null.ins
	$(CC) $(template_CFLAGS) -c template.c

yasep.o: yasep.c common.ins null.ins yasep.h yasep.ins yasep.reg
	$(CC) $(yasep_CFLAGS) -c yasep.c

yasep16.o: yasep16.c common.ins null.ins yasep.c yasep.h yasep.ins yasep.reg
	$(CC) $(yasep16_CFLAGS) -c yasep16.c

yasep32.o: yasep32.c common.ins null.ins yasep.c yasep.h yasep.ins yasep.reg
	$(CC) $(yasep32_CFLAGS) -c yasep32.c

clean:
	$(RM) -- $(amd64_OBJS) $(arm_OBJS) $(armeb_OBJS) $(armel_OBJS) $(dalvik_OBJS) $(i386_OBJS) $(i386_real_OBJS) $(i486_OBJS) $(i586_OBJS) $(i686_OBJS) $(java_OBJS) $(mips_OBJS) $(mipseb_OBJS) $(mipsel_OBJS) $(sparc_OBJS) $(sparc64_OBJS) $(template_OBJS) $(yasep_OBJS) $(yasep16_OBJS) $(yasep32_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 amd64.so $(DESTDIR)$(LIBDIR)/Asm/arch/amd64.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 arm.so $(DESTDIR)$(LIBDIR)/Asm/arch/arm.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 armeb.so $(DESTDIR)$(LIBDIR)/Asm/arch/armeb.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 armel.so $(DESTDIR)$(LIBDIR)/Asm/arch/armel.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 dalvik.so $(DESTDIR)$(LIBDIR)/Asm/arch/dalvik.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 i386.so $(DESTDIR)$(LIBDIR)/Asm/arch/i386.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 i386_real.so $(DESTDIR)$(LIBDIR)/Asm/arch/i386_real.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 i486.so $(DESTDIR)$(LIBDIR)/Asm/arch/i486.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 i586.so $(DESTDIR)$(LIBDIR)/Asm/arch/i586.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 i686.so $(DESTDIR)$(LIBDIR)/Asm/arch/i686.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 java.so $(DESTDIR)$(LIBDIR)/Asm/arch/java.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 mips.so $(DESTDIR)$(LIBDIR)/Asm/arch/mips.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 mipseb.so $(DESTDIR)$(LIBDIR)/Asm/arch/mipseb.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 mipsel.so $(DESTDIR)$(LIBDIR)/Asm/arch/mipsel.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 sparc.so $(DESTDIR)$(LIBDIR)/Asm/arch/sparc.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 sparc64.so $(DESTDIR)$(LIBDIR)/Asm/arch/sparc64.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 template.so $(DESTDIR)$(LIBDIR)/Asm/arch/template.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 yasep.so $(DESTDIR)$(LIBDIR)/Asm/arch/yasep.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 yasep16.so $(DESTDIR)$(LIBDIR)/Asm/arch/yasep16.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0644 yasep32.so $(DESTDIR)$(LIBDIR)/Asm/arch/yasep32.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/amd64.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/arm.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/armeb.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/armel.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/dalvik.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/i386.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/i386_real.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/i486.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/i586.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/i686.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/java.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/mips.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/mipseb.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/mipsel.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/sparc.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/sparc64.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/template.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/yasep.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/yasep16.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/yasep32.so

.PHONY: all clean distclean install uninstall
