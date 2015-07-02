TARGETS	= $(OBJDIR)amd64.so $(OBJDIR)arm.so $(OBJDIR)armeb.so $(OBJDIR)armel.so $(OBJDIR)dalvik.so $(OBJDIR)i386.so $(OBJDIR)i386_real.so $(OBJDIR)i486.so $(OBJDIR)i586.so $(OBJDIR)i686.so $(OBJDIR)java.so $(OBJDIR)mips.so $(OBJDIR)mipseb.so $(OBJDIR)mipsel.so $(OBJDIR)sparc.so $(OBJDIR)sparc64.so $(OBJDIR)template.so $(OBJDIR)yasep.so $(OBJDIR)yasep16.so $(OBJDIR)yasep32.so
OBJDIR	=
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

amd64_OBJS = $(OBJDIR)amd64.o
amd64_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
amd64_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)amd64.so: $(amd64_OBJS)
	$(CCSHARED) -o $(OBJDIR)amd64.so $(amd64_OBJS) $(amd64_LDFLAGS)

arm_OBJS = $(OBJDIR)arm.o
arm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
arm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)arm.so: $(arm_OBJS)
	$(CCSHARED) -o $(OBJDIR)arm.so $(arm_OBJS) $(arm_LDFLAGS)

armeb_OBJS = $(OBJDIR)armeb.o
armeb_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
armeb_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)armeb.so: $(armeb_OBJS)
	$(CCSHARED) -o $(OBJDIR)armeb.so $(armeb_OBJS) $(armeb_LDFLAGS)

armel_OBJS = $(OBJDIR)armel.o
armel_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
armel_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)armel.so: $(armel_OBJS)
	$(CCSHARED) -o $(OBJDIR)armel.so $(armel_OBJS) $(armel_LDFLAGS)

dalvik_OBJS = $(OBJDIR)dalvik.o
dalvik_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
dalvik_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)dalvik.so: $(dalvik_OBJS)
	$(CCSHARED) -o $(OBJDIR)dalvik.so $(dalvik_OBJS) $(dalvik_LDFLAGS)

i386_OBJS = $(OBJDIR)i386.o
i386_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)i386.so: $(i386_OBJS)
	$(CCSHARED) -o $(OBJDIR)i386.so $(i386_OBJS) $(i386_LDFLAGS)

i386_real_OBJS = $(OBJDIR)i386_real.o
i386_real_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_real_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)i386_real.so: $(i386_real_OBJS)
	$(CCSHARED) -o $(OBJDIR)i386_real.so $(i386_real_OBJS) $(i386_real_LDFLAGS)

i486_OBJS = $(OBJDIR)i486.o
i486_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i486_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)i486.so: $(i486_OBJS)
	$(CCSHARED) -o $(OBJDIR)i486.so $(i486_OBJS) $(i486_LDFLAGS)

i586_OBJS = $(OBJDIR)i586.o
i586_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i586_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)i586.so: $(i586_OBJS)
	$(CCSHARED) -o $(OBJDIR)i586.so $(i586_OBJS) $(i586_LDFLAGS)

i686_OBJS = $(OBJDIR)i686.o
i686_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i686_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)i686.so: $(i686_OBJS)
	$(CCSHARED) -o $(OBJDIR)i686.so $(i686_OBJS) $(i686_LDFLAGS)

java_OBJS = $(OBJDIR)java.o
java_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
java_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)java.so: $(java_OBJS)
	$(CCSHARED) -o $(OBJDIR)java.so $(java_OBJS) $(java_LDFLAGS)

mips_OBJS = $(OBJDIR)mips.o
mips_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mips_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)mips.so: $(mips_OBJS)
	$(CCSHARED) -o $(OBJDIR)mips.so $(mips_OBJS) $(mips_LDFLAGS)

mipseb_OBJS = $(OBJDIR)mipseb.o
mipseb_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mipseb_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)mipseb.so: $(mipseb_OBJS)
	$(CCSHARED) -o $(OBJDIR)mipseb.so $(mipseb_OBJS) $(mipseb_LDFLAGS)

mipsel_OBJS = $(OBJDIR)mipsel.o
mipsel_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mipsel_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)mipsel.so: $(mipsel_OBJS)
	$(CCSHARED) -o $(OBJDIR)mipsel.so $(mipsel_OBJS) $(mipsel_LDFLAGS)

sparc_OBJS = $(OBJDIR)sparc.o
sparc_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)sparc.so: $(sparc_OBJS)
	$(CCSHARED) -o $(OBJDIR)sparc.so $(sparc_OBJS) $(sparc_LDFLAGS)

sparc64_OBJS = $(OBJDIR)sparc64.o
sparc64_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc64_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)sparc64.so: $(sparc64_OBJS)
	$(CCSHARED) -o $(OBJDIR)sparc64.so $(sparc64_OBJS) $(sparc64_LDFLAGS)

template_OBJS = $(OBJDIR)template.o
template_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
template_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)template.so: $(template_OBJS)
	$(CCSHARED) -o $(OBJDIR)template.so $(template_OBJS) $(template_LDFLAGS)

yasep_OBJS = $(OBJDIR)yasep.o
yasep_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
yasep_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)yasep.so: $(yasep_OBJS)
	$(CCSHARED) -o $(OBJDIR)yasep.so $(yasep_OBJS) $(yasep_LDFLAGS)

yasep16_OBJS = $(OBJDIR)yasep16.o
yasep16_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
yasep16_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)yasep16.so: $(yasep16_OBJS)
	$(CCSHARED) -o $(OBJDIR)yasep16.so $(yasep16_OBJS) $(yasep16_LDFLAGS)

yasep32_OBJS = $(OBJDIR)yasep32.o
yasep32_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
yasep32_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)yasep32.so: $(yasep32_OBJS)
	$(CCSHARED) -o $(OBJDIR)yasep32.so $(yasep32_OBJS) $(yasep32_LDFLAGS)

$(OBJDIR)amd64.o: amd64.c amd64.ins amd64.reg common.ins null.ins i386.h i386.ins i386.reg i486.ins i686.ins i686.reg
	$(CC) $(amd64_CFLAGS) -o $(OBJDIR)amd64.o -c amd64.c

$(OBJDIR)arm.o: arm.c arm.h arm.ins arm.reg common.ins null.ins
	$(CC) $(arm_CFLAGS) -o $(OBJDIR)arm.o -c arm.c

$(OBJDIR)armeb.o: armeb.c $(OBJDIR)arm.o
	$(CC) $(armeb_CFLAGS) -o $(OBJDIR)armeb.o -c armeb.c

$(OBJDIR)armel.o: armel.c $(OBJDIR)arm.o
	$(CC) $(armel_CFLAGS) -o $(OBJDIR)armel.o -c armel.c

$(OBJDIR)dalvik.o: dalvik.c common.ins null.ins dalvik.ins dalvik.reg
	$(CC) $(dalvik_CFLAGS) -o $(OBJDIR)dalvik.o -c dalvik.c

$(OBJDIR)i386.o: i386.c common.ins null.ins i386.h i386.ins i386.reg
	$(CC) $(i386_CFLAGS) -o $(OBJDIR)i386.o -c i386.c

$(OBJDIR)i386_real.o: i386_real.c common.ins null.ins i386.h i386.ins i386.reg
	$(CC) $(i386_real_CFLAGS) -o $(OBJDIR)i386_real.o -c i386_real.c

$(OBJDIR)i486.o: i486.c common.ins null.ins i386.h i386.ins i386.reg i486.ins
	$(CC) $(i486_CFLAGS) -o $(OBJDIR)i486.o -c i486.c

$(OBJDIR)i586.o: i586.c common.ins null.ins i386.h i386.ins i386.reg i486.ins i586.ins
	$(CC) $(i586_CFLAGS) -o $(OBJDIR)i586.o -c i586.c

$(OBJDIR)i686.o: i686.c common.ins null.ins i386.h i386.ins i386.reg i486.ins i686.ins i686.reg
	$(CC) $(i686_CFLAGS) -o $(OBJDIR)i686.o -c i686.c

$(OBJDIR)java.o: java.c common.ins null.ins
	$(CC) $(java_CFLAGS) -o $(OBJDIR)java.o -c java.c

$(OBJDIR)mips.o: mips.c common.ins null.ins mips.h mips.ins mips.reg
	$(CC) $(mips_CFLAGS) -o $(OBJDIR)mips.o -c mips.c

$(OBJDIR)mipseb.o: mipseb.c $(OBJDIR)mips.o
	$(CC) $(mipseb_CFLAGS) -o $(OBJDIR)mipseb.o -c mipseb.c

$(OBJDIR)mipsel.o: mipsel.c $(OBJDIR)mips.o
	$(CC) $(mipsel_CFLAGS) -o $(OBJDIR)mipsel.o -c mipsel.c

$(OBJDIR)sparc.o: sparc.c common.ins null.ins sparc.h sparc.ins sparc.reg
	$(CC) $(sparc_CFLAGS) -o $(OBJDIR)sparc.o -c sparc.c

$(OBJDIR)sparc64.o: sparc64.c common.ins null.ins sparc.h sparc.ins sparc.reg
	$(CC) $(sparc64_CFLAGS) -o $(OBJDIR)sparc64.o -c sparc64.c

$(OBJDIR)template.o: template.c common.ins null.ins
	$(CC) $(template_CFLAGS) -o $(OBJDIR)template.o -c template.c

$(OBJDIR)yasep.o: yasep.c common.ins null.ins yasep.h yasep.ins yasep.reg
	$(CC) $(yasep_CFLAGS) -o $(OBJDIR)yasep.o -c yasep.c

$(OBJDIR)yasep16.o: yasep16.c common.ins null.ins yasep.c yasep.h yasep.ins yasep.reg
	$(CC) $(yasep16_CFLAGS) -o $(OBJDIR)yasep16.o -c yasep16.c

$(OBJDIR)yasep32.o: yasep32.c common.ins null.ins yasep.c yasep.h yasep.ins yasep.reg
	$(CC) $(yasep32_CFLAGS) -o $(OBJDIR)yasep32.o -c yasep32.c

clean:
	$(RM) -- $(amd64_OBJS) $(arm_OBJS) $(armeb_OBJS) $(armel_OBJS) $(dalvik_OBJS) $(i386_OBJS) $(i386_real_OBJS) $(i486_OBJS) $(i586_OBJS) $(i686_OBJS) $(java_OBJS) $(mips_OBJS) $(mipseb_OBJS) $(mipsel_OBJS) $(sparc_OBJS) $(sparc64_OBJS) $(template_OBJS) $(yasep_OBJS) $(yasep16_OBJS) $(yasep32_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)amd64.so $(DESTDIR)$(LIBDIR)/Asm/arch/amd64.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)arm.so $(DESTDIR)$(LIBDIR)/Asm/arch/arm.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)armeb.so $(DESTDIR)$(LIBDIR)/Asm/arch/armeb.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)armel.so $(DESTDIR)$(LIBDIR)/Asm/arch/armel.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)dalvik.so $(DESTDIR)$(LIBDIR)/Asm/arch/dalvik.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)i386.so $(DESTDIR)$(LIBDIR)/Asm/arch/i386.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)i386_real.so $(DESTDIR)$(LIBDIR)/Asm/arch/i386_real.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)i486.so $(DESTDIR)$(LIBDIR)/Asm/arch/i486.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)i586.so $(DESTDIR)$(LIBDIR)/Asm/arch/i586.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)i686.so $(DESTDIR)$(LIBDIR)/Asm/arch/i686.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)java.so $(DESTDIR)$(LIBDIR)/Asm/arch/java.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)mips.so $(DESTDIR)$(LIBDIR)/Asm/arch/mips.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)mipseb.so $(DESTDIR)$(LIBDIR)/Asm/arch/mipseb.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)mipsel.so $(DESTDIR)$(LIBDIR)/Asm/arch/mipsel.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)sparc.so $(DESTDIR)$(LIBDIR)/Asm/arch/sparc.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)sparc64.so $(DESTDIR)$(LIBDIR)/Asm/arch/sparc64.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)yasep.so $(DESTDIR)$(LIBDIR)/Asm/arch/yasep.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)yasep16.so $(DESTDIR)$(LIBDIR)/Asm/arch/yasep16.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/arch
	$(INSTALL) -m 0755 $(OBJDIR)yasep32.so $(DESTDIR)$(LIBDIR)/Asm/arch/yasep32.so

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
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/yasep.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/yasep16.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/arch/yasep32.so

.PHONY: all clean distclean install uninstall
