OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f
TARGETS	= $(OBJDIR)_libAsm$(SOEXT)
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= `pkg-config --cflags python-2.7 cpp` -fPIC
CFLAGS	= -W -Wall -g -O2 -D_FORTIFY_SOURCE=2 -fstack-protector
LDFLAGSF= `pkg-config --libs python-2.7 cpp` -lAsm
LDFLAGS	= -L$(OBJDIR).. -Wl,-rpath,$(LIBDIR)
EXEEXT	=
AR	= ar
ARFLAGS	= -rc
RANLIB	= ranlib
CCSHARED= $(CC) -shared
SOEXT	= .so
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

_libAsm_OBJS = $(OBJDIR)libAsm.o
_libAsm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
_libAsm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)_libAsm$(SOEXT): $(_libAsm_OBJS)
	$(CCSHARED) -o $(OBJDIR)_libAsm$(SOEXT) $(_libAsm_OBJS) $(_libAsm_LDFLAGS)

$(OBJDIR)libAsm.o: libAsm.c
	$(CC) $(_libAsm_CFLAGS) -o $(OBJDIR)libAsm.o -c libAsm.c

clean:
	$(RM) -- $(_libAsm_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: all
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0755 $(OBJDIR)_libAsm$(SOEXT) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libAsm$(SOEXT)
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0644 libAsm.py $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libAsm.py

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libAsm$(SOEXT)
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libAsm.py

.PHONY: all clean distclean install uninstall
