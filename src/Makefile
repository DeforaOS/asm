SUBDIRS	= arch format
TARGETS	= libasm.a libasm.so asm deasm
PREFIX	= /usr/local
DESTDIR	= 
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS= 
CFLAGSF	= -W `pkg-config --cflags cpp`
CFLAGS	= -Wall -g -O2 -pedantic
LDFLAGSF= 
AR	= ar -rc
RANLIB	= ranlib
LD	= $(CC) -shared
BINDIR	= $(PREFIX)/bin
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: subdirs $(TARGETS)

subdirs:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE)) || exit; done

libasm_OBJS = arch.o asm.o code.o format.o parser.o token.o
libasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) -fPIC
libasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs cpp`

libasm.a: $(libasm_OBJS)
	$(AR) libasm.a $(libasm_OBJS)
	$(RANLIB) libasm.a

libasm.so: $(libasm_OBJS)
	$(LD) -o libasm.so -Wl,-soname,libasm.so.0 $(libasm_OBJS) $(libasm_LDFLAGS)

asm_OBJS = main.o
asm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
asm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L. -Wl,-rpath,$(LIBDIR) -lasm

asm: $(asm_OBJS) libasm.so
	$(CC) -o asm $(asm_OBJS) $(asm_LDFLAGS)

deasm_OBJS = deasm.o
deasm_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
deasm_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L. -Wl,-rpath,$(LIBDIR) -lasm

deasm: $(deasm_OBJS) libasm.so
	$(CC) -o deasm $(deasm_OBJS) $(deasm_LDFLAGS)

arch.o: arch.c arch.h ../config.h
	$(CC) $(libasm_CFLAGS) -c arch.c

asm.o: asm.c ../include/Asm.h code.h parser.h ../config.h
	$(CC) $(libasm_CFLAGS) -c asm.c

code.o: code.c ../include/Asm.h arch.h code.h
	$(CC) $(libasm_CFLAGS) -c code.c

format.o: format.c format.h ../config.h
	$(CC) $(libasm_CFLAGS) -c format.c

parser.o: parser.c parser.h
	$(CC) $(libasm_CFLAGS) -c parser.c

token.o: token.c token.h
	$(CC) $(libasm_CFLAGS) -c token.c

main.o: main.c ../include/Asm.h
	$(CC) $(asm_CFLAGS) -c main.c

deasm.o: deasm.c ../include/Asm.h
	$(CC) $(deasm_CFLAGS) -c deasm.c

clean:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) clean) || exit; done
	$(RM) -- $(libasm_OBJS) $(asm_OBJS) $(deasm_OBJS)

distclean:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) distclean) || exit; done
	$(RM) -- $(libasm_OBJS) $(asm_OBJS) $(deasm_OBJS)
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) install) || exit; done
	$(MKDIR) $(DESTDIR)$(LIBDIR)
	$(INSTALL) -m 0644 -- libasm.a $(DESTDIR)$(LIBDIR)/libasm.a
	$(INSTALL) -m 0755 -- libasm.so $(DESTDIR)$(LIBDIR)/libasm.so.0.0
	$(LN) -s -- libasm.so.0.0 $(DESTDIR)$(LIBDIR)/libasm.so.0
	$(LN) -s -- libasm.so.0.0 $(DESTDIR)$(LIBDIR)/libasm.so
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 -- asm $(DESTDIR)$(BINDIR)/asm
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 -- deasm $(DESTDIR)$(BINDIR)/deasm

uninstall:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) uninstall) || exit; done
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.a
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.so.0.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.so.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libasm.so
	$(RM) -- $(DESTDIR)$(BINDIR)/asm
	$(RM) -- $(DESTDIR)$(BINDIR)/deasm

.PHONY: all subdirs clean distclean install uninstall
