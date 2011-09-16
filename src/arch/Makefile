TARGETS	= amd64.so arm.so dalvik.so i386.so i386_real.so i486.so i586.so i686.so java.so mips.so sparc.so sparc64.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libSystem`
CFLAGS	= -Wall -g -O2 -fPIC -pedantic
AR	= ar -rc
RANLIB	= ranlib
LD	= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

amd64_OBJS = amd64.o
amd64_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
amd64_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

amd64.so: $(amd64_OBJS)
	$(LD) -o amd64.so $(amd64_OBJS) $(amd64_LDFLAGS)

arm_OBJS = arm.o
arm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
arm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

arm.so: $(arm_OBJS)
	$(LD) -o arm.so $(arm_OBJS) $(arm_LDFLAGS)

dalvik_OBJS = dalvik.o
dalvik_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
dalvik_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

dalvik.so: $(dalvik_OBJS)
	$(LD) -o dalvik.so $(dalvik_OBJS) $(dalvik_LDFLAGS)

i386_OBJS = i386.o
i386_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i386.so: $(i386_OBJS)
	$(LD) -o i386.so $(i386_OBJS) $(i386_LDFLAGS)

i386_real_OBJS = i386_real.o
i386_real_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_real_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i386_real.so: $(i386_real_OBJS)
	$(LD) -o i386_real.so $(i386_real_OBJS) $(i386_real_LDFLAGS)

i486_OBJS = i486.o
i486_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i486_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i486.so: $(i486_OBJS)
	$(LD) -o i486.so $(i486_OBJS) $(i486_LDFLAGS)

i586_OBJS = i586.o
i586_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i586_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i586.so: $(i586_OBJS)
	$(LD) -o i586.so $(i586_OBJS) $(i586_LDFLAGS)

i686_OBJS = i686.o
i686_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i686_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i686.so: $(i686_OBJS)
	$(LD) -o i686.so $(i686_OBJS) $(i686_LDFLAGS)

java_OBJS = java.o
java_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
java_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

java.so: $(java_OBJS)
	$(LD) -o java.so $(java_OBJS) $(java_LDFLAGS)

mips_OBJS = mips.o
mips_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
mips_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

mips.so: $(mips_OBJS)
	$(LD) -o mips.so $(mips_OBJS) $(mips_LDFLAGS)

sparc_OBJS = sparc.o
sparc_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

sparc.so: $(sparc_OBJS)
	$(LD) -o sparc.so $(sparc_OBJS) $(sparc_LDFLAGS)

sparc64_OBJS = sparc64.o
sparc64_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc64_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

sparc64.so: $(sparc64_OBJS)
	$(LD) -o sparc64.so $(sparc64_OBJS) $(sparc64_LDFLAGS)

amd64.o: amd64.c amd64.ins amd64.reg common.ins null.ins i386.h i386.ins i386.reg i486.ins i686.ins i686.reg
	$(CC) $(amd64_CFLAGS) -c amd64.c

arm.o: arm.c arm.h arm.ins arm.reg common.ins null.ins
	$(CC) $(arm_CFLAGS) -c arm.c

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

sparc.o: sparc.c common.ins null.ins sparc.h sparc.ins sparc.reg
	$(CC) $(sparc_CFLAGS) -c sparc.c

sparc64.o: sparc64.c common.ins null.ins sparc.h sparc.ins sparc.reg
	$(CC) $(sparc64_CFLAGS) -c sparc64.c

clean:
	$(RM) -- $(amd64_OBJS) $(arm_OBJS) $(dalvik_OBJS) $(i386_OBJS) $(i386_real_OBJS) $(i486_OBJS) $(i586_OBJS) $(i686_OBJS) $(java_OBJS) $(mips_OBJS) $(sparc_OBJS) $(sparc64_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- amd64.so $(DESTDIR)$(LIBDIR)/asm/arch/amd64.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- arm.so $(DESTDIR)$(LIBDIR)/asm/arch/arm.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- dalvik.so $(DESTDIR)$(LIBDIR)/asm/arch/dalvik.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i386.so $(DESTDIR)$(LIBDIR)/asm/arch/i386.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i386_real.so $(DESTDIR)$(LIBDIR)/asm/arch/i386_real.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i486.so $(DESTDIR)$(LIBDIR)/asm/arch/i486.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i586.so $(DESTDIR)$(LIBDIR)/asm/arch/i586.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i686.so $(DESTDIR)$(LIBDIR)/asm/arch/i686.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- java.so $(DESTDIR)$(LIBDIR)/asm/arch/java.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- mips.so $(DESTDIR)$(LIBDIR)/asm/arch/mips.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- sparc.so $(DESTDIR)$(LIBDIR)/asm/arch/sparc.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- sparc64.so $(DESTDIR)$(LIBDIR)/asm/arch/sparc64.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/amd64.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/arm.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/dalvik.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i386.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i386_real.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i486.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i586.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i686.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/java.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/mips.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/sparc.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/sparc64.so

.PHONY: all clean distclean install uninstall
