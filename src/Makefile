SUBDIRS	= arch format
TARGETS	= $(OBJDIR)libAsm.a $(OBJDIR)libAsm.so.0.0 $(OBJDIR)libAsm.so.0 $(OBJDIR)libAsm.so $(OBJDIR)asm $(OBJDIR)deasm
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags cpp`
CFLAGS	= -Wall -g -O2 -pedantic
LDFLAGSF= `pkg-config --libs cpp`
LDFLAGS	= 
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: subdirs $(TARGETS)

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		([ -d "$(OBJDIR)$$i" ] || $(MKDIR) -- "$(OBJDIR)$$i") && \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/"; \
		else $(MAKE); fi) || exit; done

libAsm_OBJS = $(OBJDIR)arch.o $(OBJDIR)asm.o $(OBJDIR)code.o $(OBJDIR)format.o $(OBJDIR)parser.o $(OBJDIR)token.o
libAsm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) -fPIC
libAsm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)libAsm.a: $(libAsm_OBJS)
	$(AR) -rc $(OBJDIR)libAsm.a $(libAsm_OBJS)
	$(RANLIB) $(OBJDIR)libAsm.a

$(OBJDIR)libAsm.so.0.0: $(libAsm_OBJS)
	$(CCSHARED) -o $(OBJDIR)libAsm.so.0.0 -Wl,-soname,libAsm.so.0 $(libAsm_OBJS) $(libAsm_LDFLAGS)

$(OBJDIR)libAsm.so.0: $(OBJDIR)libAsm.so.0.0
	$(LN) -s -- libAsm.so.0.0 $(OBJDIR)libAsm.so.0

$(OBJDIR)libAsm.so: $(OBJDIR)libAsm.so.0.0
	$(LN) -s -- libAsm.so.0.0 $(OBJDIR)libAsm.so

asm_OBJS = $(OBJDIR)main.o
asm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
asm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L$(OBJDIR). -Wl,-rpath,$(LIBDIR) -lAsm

$(OBJDIR)asm: $(asm_OBJS) $(OBJDIR)libAsm.so
	$(CC) -o $(OBJDIR)asm $(asm_OBJS) $(asm_LDFLAGS)

deasm_OBJS = $(OBJDIR)deasm.o
deasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
deasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L$(OBJDIR). -Wl,-rpath,$(LIBDIR) -lAsm

$(OBJDIR)deasm: $(deasm_OBJS) $(OBJDIR)libAsm.so
	$(CC) -o $(OBJDIR)deasm $(deasm_OBJS) $(deasm_LDFLAGS)

$(OBJDIR)arch.o: arch.c ../include/Asm/arch.h ../include/Asm/asm.h arch.h code.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libAsm_CFLAGS) -o $(OBJDIR)arch.o -c arch.c

$(OBJDIR)asm.o: asm.c ../include/Asm.h code.h parser.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libAsm_CFLAGS) -o $(OBJDIR)asm.o -c asm.c

$(OBJDIR)code.o: code.c ../include/Asm.h ../include/Asm/arch.h ../include/Asm/asm.h ../include/Asm/format.h arch.h code.h format.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libAsm_CFLAGS) -o $(OBJDIR)code.o -c code.c

$(OBJDIR)format.o: format.c ../include/Asm/asm.h ../include/Asm/format.h code.h format.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libAsm_CFLAGS) -o $(OBJDIR)format.o -c format.c

$(OBJDIR)parser.o: parser.c parser.h token.h
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)parser.o -c parser.c

$(OBJDIR)token.o: token.c common.h token.h
	$(CC) $(libAsm_CFLAGS) -o $(OBJDIR)token.o -c token.c

$(OBJDIR)main.o: main.c ../include/Asm/asm.h ../config.h
	$(CC) $(asm_CFLAGS) -o $(OBJDIR)main.o -c main.c

$(OBJDIR)deasm.o: deasm.c ../include/Asm.h ../include/Asm/arch.h ../include/Asm/asm.h ../include/Asm/format.h
	$(CC) $(deasm_CFLAGS) -o $(OBJDIR)deasm.o -c deasm.c

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" clean; \
		else $(MAKE) clean; fi) || exit; done
	$(RM) -- $(libAsm_OBJS) $(asm_OBJS) $(deasm_OBJS)

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" distclean; \
		else $(MAKE) distclean; fi) || exit; done
	$(RM) -- $(libAsm_OBJS) $(asm_OBJS) $(deasm_OBJS)
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" install; \
		else $(MAKE) install; fi) || exit; done
	$(MKDIR) $(DESTDIR)$(LIBDIR)
	$(INSTALL) -m 0644 $(OBJDIR)libAsm.a $(DESTDIR)$(LIBDIR)/libAsm.a
	$(INSTALL) -m 0755 $(OBJDIR)libAsm.so.0.0 $(DESTDIR)$(LIBDIR)/libAsm.so.0.0
	$(LN) -s -- libAsm.so.0.0 $(DESTDIR)$(LIBDIR)/libAsm.so.0
	$(LN) -s -- libAsm.so.0.0 $(DESTDIR)$(LIBDIR)/libAsm.so
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)asm $(DESTDIR)$(BINDIR)/asm
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)deasm $(DESTDIR)$(BINDIR)/deasm

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" uninstall; \
		else $(MAKE) uninstall; fi) || exit; done
	$(RM) -- $(DESTDIR)$(LIBDIR)/libAsm.a
	$(RM) -- $(DESTDIR)$(LIBDIR)/libAsm.so.0.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libAsm.so.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libAsm.so
	$(RM) -- $(DESTDIR)$(BINDIR)/asm
	$(RM) -- $(DESTDIR)$(BINDIR)/deasm

.PHONY: all subdirs clean distclean install uninstall
