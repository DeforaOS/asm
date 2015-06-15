MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f
TARGETS	= $(OBJDIR)_libasm.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags python-2.7 Asm` -fPIC
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs python-2.7 Asm`
LDFLAGS	= -L.. -L$(LIBDIR) -Wl,-rpath,$(LIBDIR)
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

_libasm_OBJS = $(OBJDIR)libasm.o
_libasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
_libasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)_libasm.so: $(_libasm_OBJS)
	$(CCSHARED) -o $(OBJDIR)_libasm.so $(_libasm_OBJS) $(_libasm_LDFLAGS)

$(OBJDIR)libasm.o: libasm.c
	$(CC) $(_libasm_CFLAGS) -o $(OBJDIR)libasm.o -c libasm.c

clean:
	$(RM) -- $(_libasm_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0755 $(OBJDIR)_libasm.so $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libasm.so
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0644 libasm.py $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libasm.py

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libasm.so
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libasm.py

.PHONY: all clean distclean install uninstall
