TARGETS	= i386.so i486.so sparc.so
PREFIX	= /usr/local
DESTDIR	= 
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W
CFLAGS	= -Wall -fPIC -pedantic
AR	= ar -rc
RANLIB	= ranlib
LD	= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

i386_OBJS = i386.o
i386_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i386_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i386.so: $(i386_OBJS)
	$(LD) -o i386.so $(i386_OBJS) $(i386_LDFLAGS)

i486_OBJS = i486.o
i486_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
i486_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

i486.so: $(i486_OBJS)
	$(LD) -o i486.so $(i486_OBJS) $(i486_LDFLAGS)

sparc_OBJS = sparc.o
sparc_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

sparc.so: $(sparc_OBJS)
	$(LD) -o sparc.so $(sparc_OBJS) $(sparc_LDFLAGS)

i386.o: i386.c common.ins null.ins i386.h i386.ins i386.reg
	$(CC) $(i386_CFLAGS) -c i386.c

i486.o: i486.c common.ins null.ins i386.h i386.ins i386.reg i486.ins
	$(CC) $(i486_CFLAGS) -c i486.c

sparc.o: sparc.c common.ins null.ins sparc.ins sparc.reg
	$(CC) $(sparc_CFLAGS) -c sparc.c

clean:
	$(RM) -- $(i386_OBJS) $(i486_OBJS) $(sparc_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i386.so $(DESTDIR)$(LIBDIR)/asm/arch/i386.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- i486.so $(DESTDIR)$(LIBDIR)/asm/arch/i486.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- sparc.so $(DESTDIR)$(LIBDIR)/asm/arch/sparc.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i386.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/i486.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/sparc.so

.PHONY: all clean distclean install uninstall
