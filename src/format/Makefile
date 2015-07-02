TARGETS	= $(OBJDIR)dex.so $(OBJDIR)elf.so $(OBJDIR)flat.so $(OBJDIR)java.so $(OBJDIR)pe.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libSystem`
CFLAGS	= -Wall -g -O2 -fPIC -pedantic
LDFLAGSF= -W `pkg-config --libs libSystem`
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

dex_OBJS = $(OBJDIR)dex.o
dex_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
dex_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)dex.so: $(dex_OBJS)
	$(CCSHARED) -o $(OBJDIR)dex.so $(dex_OBJS) $(dex_LDFLAGS)

elf_OBJS = $(OBJDIR)elf.o $(OBJDIR)elf/common.o $(OBJDIR)elf/elf32.o $(OBJDIR)elf/elf64.o
elf_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
elf_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)elf.so: $(elf_OBJS)
	$(CCSHARED) -o $(OBJDIR)elf.so $(elf_OBJS) $(elf_LDFLAGS)

flat_OBJS = $(OBJDIR)flat.o
flat_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
flat_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)flat.so: $(flat_OBJS)
	$(CCSHARED) -o $(OBJDIR)flat.so $(flat_OBJS) $(flat_LDFLAGS)

java_OBJS = $(OBJDIR)java.o
java_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
java_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)java.so: $(java_OBJS)
	$(CCSHARED) -o $(OBJDIR)java.so $(java_OBJS) $(java_LDFLAGS)

pe_OBJS = $(OBJDIR)pe.o
pe_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
pe_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)pe.so: $(pe_OBJS)
	$(CCSHARED) -o $(OBJDIR)pe.so $(pe_OBJS) $(pe_LDFLAGS)

$(OBJDIR)dex.o: dex.c
	$(CC) $(dex_CFLAGS) -o $(OBJDIR)dex.o -c dex.c

$(OBJDIR)elf.o: elf.c elf/common.h
	$(CC) $(elf_CFLAGS) -o $(OBJDIR)elf.o -c elf.c

$(OBJDIR)elf/common.o: elf/common.c elf/common.h
	@[ -d "$(OBJDIR)elf" ] || $(MKDIR) -- "$(OBJDIR)elf"
	$(CC) $(elf_CFLAGS) -o $(OBJDIR)elf/common.o -c elf/common.c

$(OBJDIR)elf/elf32.o: elf/elf32.c elf/common.h elf/format.c
	@[ -d "$(OBJDIR)elf" ] || $(MKDIR) -- "$(OBJDIR)elf"
	$(CC) $(elf_CFLAGS) -o $(OBJDIR)elf/elf32.o -c elf/elf32.c

$(OBJDIR)elf/elf64.o: elf/elf64.c elf/common.h elf/format.c
	@[ -d "$(OBJDIR)elf" ] || $(MKDIR) -- "$(OBJDIR)elf"
	$(CC) $(elf_CFLAGS) -o $(OBJDIR)elf/elf64.o -c elf/elf64.c

$(OBJDIR)flat.o: flat.c
	$(CC) $(flat_CFLAGS) -o $(OBJDIR)flat.o -c flat.c

$(OBJDIR)java.o: java.c
	$(CC) $(java_CFLAGS) -o $(OBJDIR)java.o -c java.c

$(OBJDIR)pe.o: pe.c
	$(CC) $(pe_CFLAGS) -o $(OBJDIR)pe.o -c pe.c

clean:
	$(RM) -- $(dex_OBJS) $(elf_OBJS) $(flat_OBJS) $(java_OBJS) $(pe_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/format
	$(INSTALL) -m 0755 $(OBJDIR)dex.so $(DESTDIR)$(LIBDIR)/Asm/format/dex.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/format
	$(INSTALL) -m 0755 $(OBJDIR)elf.so $(DESTDIR)$(LIBDIR)/Asm/format/elf.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/format
	$(INSTALL) -m 0755 $(OBJDIR)flat.so $(DESTDIR)$(LIBDIR)/Asm/format/flat.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/format
	$(INSTALL) -m 0755 $(OBJDIR)java.so $(DESTDIR)$(LIBDIR)/Asm/format/java.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Asm/format
	$(INSTALL) -m 0755 $(OBJDIR)pe.so $(DESTDIR)$(LIBDIR)/Asm/format/pe.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/format/dex.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/format/elf.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/format/flat.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/format/java.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Asm/format/pe.so

.PHONY: all clean distclean install uninstall
