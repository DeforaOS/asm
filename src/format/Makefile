TARGETS	= dex.so elf.so flat.so java.so pe.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	?= cc
CPPFLAGSF= -I ../../include
CPPFLAGS?=
CFLAGSF	= -W `pkg-config --cflags libSystem`
CFLAGS	= -Wall -g -O2 -fPIC -pedantic
AR	?= ar
RANLIB	?= ranlib
CCSHARED?= $(CC) -shared
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install


all: $(TARGETS)

dex_OBJS = dex.o
dex_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
dex_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

dex.so: $(dex_OBJS)
	$(CCSHARED) -o dex.so $(dex_OBJS) $(dex_LDFLAGS)

elf_OBJS = elf.o
elf_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
elf_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

elf.so: $(elf_OBJS)
	$(CCSHARED) -o elf.so $(elf_OBJS) $(elf_LDFLAGS)

flat_OBJS = flat.o
flat_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
flat_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

flat.so: $(flat_OBJS)
	$(CCSHARED) -o flat.so $(flat_OBJS) $(flat_LDFLAGS)

java_OBJS = java.o
java_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
java_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

java.so: $(java_OBJS)
	$(CCSHARED) -o java.so $(java_OBJS) $(java_LDFLAGS)

pe_OBJS = pe.o
pe_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
pe_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

pe.so: $(pe_OBJS)
	$(CCSHARED) -o pe.so $(pe_OBJS) $(pe_LDFLAGS)

dex.o: dex.c
	$(CC) $(dex_CFLAGS) -c dex.c

elf.o: elf.c
	$(CC) $(elf_CFLAGS) -c elf.c

flat.o: flat.c
	$(CC) $(flat_CFLAGS) -c flat.c

java.o: java.c
	$(CC) $(java_CFLAGS) -c java.c

pe.o: pe.c
	$(CC) $(pe_CFLAGS) -c pe.c

clean:
	$(RM) -- $(dex_OBJS) $(elf_OBJS) $(flat_OBJS) $(java_OBJS) $(pe_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/format
	$(INSTALL) -m 0644 -- dex.so $(DESTDIR)$(LIBDIR)/asm/format/dex.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/format
	$(INSTALL) -m 0644 -- elf.so $(DESTDIR)$(LIBDIR)/asm/format/elf.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/format
	$(INSTALL) -m 0644 -- flat.so $(DESTDIR)$(LIBDIR)/asm/format/flat.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/format
	$(INSTALL) -m 0644 -- java.so $(DESTDIR)$(LIBDIR)/asm/format/java.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/asm/format
	$(INSTALL) -m 0644 -- pe.so $(DESTDIR)$(LIBDIR)/asm/format/pe.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/format/dex.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/format/elf.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/format/flat.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/format/java.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/asm/format/pe.so

.PHONY: all clean distclean install uninstall
