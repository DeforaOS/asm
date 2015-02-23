SUBDIRS	= arch format
TARGETS	= $(OBJDIR)libasm.a $(OBJDIR)libasm.so.0.0 $(OBJDIR)libasm.so.0 $(OBJDIR)libasm.so $(OBJDIR)asm $(OBJDIR)asm-static $(OBJDIR)deasm $(OBJDIR)deasm-static
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags cpp`
CFLAGS	= -Wall -g -O2 -pedantic
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

libasm_OBJS = $(OBJDIR)arch.o $(OBJDIR)asm.o $(OBJDIR)code.o $(OBJDIR)format.o $(OBJDIR)parser.o $(OBJDIR)token.o
libasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) -fPIC
libasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs cpp`

$(OBJDIR)libasm.a: $(libasm_OBJS)
	$(AR) -rc $(OBJDIR)libasm.a $(libasm_OBJS)
	$(RANLIB) $(OBJDIR)libasm.a

$(OBJDIR)libasm.so.0.0 $(OBJDIR)libasm.so.0 $(OBJDIR)libasm.so: $(libasm_OBJS)
	$(CCSHARED) -o $(OBJDIR)libasm.so.0.0 -Wl,-soname,libasm.so.0 $(libasm_OBJS) $(libasm_LDFLAGS)
	$(LN) -s -- libasm.so.0.0 $(OBJDIR)libasm.so.0
	$(LN) -s -- libasm.so.0.0 $(OBJDIR)libasm.so

asm_OBJS = $(OBJDIR)main.o
asm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
asm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L$(OBJDIR). -Wl,-rpath,$(LIBDIR) -lasm

$(OBJDIR)asm: $(asm_OBJS) $(OBJDIR)libasm.so
	$(CC) -o $(OBJDIR)asm $(asm_OBJS) $(asm_LDFLAGS)
asm-static_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
asm-static_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs cpp` $(OBJDIR)main.o $(OBJDIR)libasm.a

$(OBJDIR)asm-static: $(asm-static_OBJS) $(OBJDIR)main.o $(OBJDIR)libasm.a
	$(CC) -o $(OBJDIR)asm-static $(asm-static_OBJS) $(asm-static_LDFLAGS)

deasm_OBJS = $(OBJDIR)deasm.o
deasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
deasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L$(OBJDIR). -Wl,-rpath,$(LIBDIR) -lasm

$(OBJDIR)deasm: $(deasm_OBJS) $(OBJDIR)libasm.so
	$(CC) -o $(OBJDIR)deasm $(deasm_OBJS) $(deasm_LDFLAGS)
deasm-static_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
deasm-static_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs cpp` $(OBJDIR)deasm.o $(OBJDIR)libasm.a

$(OBJDIR)deasm-static: $(deasm-static_OBJS) $(OBJDIR)deasm.o $(OBJDIR)libasm.a
	$(CC) -o $(OBJDIR)deasm-static $(deasm-static_OBJS) $(deasm-static_LDFLAGS)

$(OBJDIR)arch.o: arch.c ../include/Asm/arch.h ../include/Asm/asm.h arch.h code.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libasm_CFLAGS) -o $(OBJDIR)arch.o -c arch.c

$(OBJDIR)asm.o: asm.c ../include/Asm.h code.h parser.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libasm_CFLAGS) -o $(OBJDIR)asm.o -c asm.c

$(OBJDIR)code.o: code.c ../include/Asm.h ../include/Asm/arch.h ../include/Asm/asm.h ../include/Asm/format.h arch.h code.h format.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libasm_CFLAGS) -o $(OBJDIR)code.o -c code.c

$(OBJDIR)format.o: format.c ../include/Asm/asm.h ../include/Asm/format.h code.h format.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(libasm_CFLAGS) -o $(OBJDIR)format.o -c format.c

$(OBJDIR)parser.o: parser.c parser.h token.h
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)parser.o -c parser.c

$(OBJDIR)token.o: token.c common.h token.h
	$(CC) $(libasm_CFLAGS) -o $(OBJDIR)token.o -c token.c

$(OBJDIR)main.o: main.c ../include/Asm/asm.h ../config.h
	$(CC) $(asm_CFLAGS) -o $(OBJDIR)main.o -c main.c

$(OBJDIR)deasm.o: deasm.c ../include/Asm.h ../include/Asm/arch.h ../include/Asm/asm.h ../include/Asm/format.h
	$(CC) $(deasm_CFLAGS) -o $(OBJDIR)deasm.o -c deasm.c

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) clean) || exit; done
	$(RM) -- $(libasm_OBJS) $(asm_OBJS) $(asm-static_OBJS) $(deasm_OBJS) $(deasm-static_OBJS)

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) distclean) || exit; done
	$(RM) -- $(libasm_OBJS) $(asm_OBJS) $(asm-static_OBJS) $(deasm_OBJS) $(deasm-static_OBJS)
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) install) || exit; done
	$(MKDIR) $(DESTDIR)$(LIBDIR)
	$(INSTALL) -m 0644 $(OBJDIR)libasm.a $(DESTDIR)$(LIBDIR)/libasm.a
	$(INSTALL) -m 0755 $(OBJDIR)libasm.so.0.0 $(DESTDIR)$(LIBDIR)/libasm.so.0.0
	$(LN) -s -- libasm.so.0.0 $(DESTDIR)$(LIBDIR)/libasm.so.0
	$(LN) -s -- libasm.so.0.0 $(DESTDIR)$(LIBDIR)/libasm.so
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)asm $(DESTDIR)$(BINDIR)/asm
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)deasm $(DESTDIR)$(BINDIR)/deasm

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) uninstall) || exit; done
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.a
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.so.0.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.so.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.so
	$(RM) -- $(DESTDIR)$(BINDIR)/asm
	$(RM) -- $(DESTDIR)$(BINDIR)/deasm

.PHONY: all subdirs clean distclean install uninstall
