TARGETS	= sparc.so
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

sparc_OBJS = sparc.o
sparc_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
sparc_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

sparc.so: $(sparc_OBJS)
	$(LD) -o sparc.so $(sparc_OBJS) $(sparc_LDFLAGS)

sparc.o: sparc.c common.ins common.reg null.ins sparc.ins sparc.reg
	$(CC) $(sparc_CFLAGS) -c sparc.c

clean:
	$(RM) -- $(sparc_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/arch
	$(INSTALL) -m 0644 -- sparc.so $(DESTDIR)$(LIBDIR)/asm/arch/sparc.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/arch/sparc.so

.PHONY: all clean distclean install uninstall
