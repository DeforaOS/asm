TARGETS	= sparc.o
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

sparc.o_OBJS = sparc.o
sparc.o_ASFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(ASFLAGS) -a sparc

sparc.o: sparc.S ../src/asm
	$(AS) $(sparc.o_ASFLAGS) -o sparc.o sparc.S

clean:
	$(RM) -- $(sparc.o_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
