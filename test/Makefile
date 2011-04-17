TARGETS	= i386.o i486.o sparc.o
PREFIX	= /usr/local
DESTDIR	= 
BINDIR	= $(PREFIX)/bin
AS	= ../src/asm
ASFLAGS	= 
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

i386.o_OBJS = i386.o
i386.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i386 -f flat

i486.o_OBJS = i486.o
i486.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a i486 -f flat

sparc.o_OBJS = sparc.o
sparc.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc -f flat

i386.o: i386.S ../src/asm
	$(AS) $(i386.o_ASFLAGS) -o i386.o i386.S

i486.o: i486.S ../src/asm
	$(AS) $(i486.o_ASFLAGS) -o i486.o i486.S

sparc.o: sparc.S ../src/asm
	$(AS) $(sparc.o_ASFLAGS) -o sparc.o sparc.S

clean:
	$(RM) -- $(i386.o_OBJS) $(i486.o_OBJS) $(sparc.o_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
