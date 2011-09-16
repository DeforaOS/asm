TARGETS	= asm.pc
PREFIX	= /usr/local
DESTDIR	=
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install


all: $(TARGETS)

asm.pc: asm.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "asm.pc"

clean:
	$(RM) -- $(asm.pc_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "asm.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "asm.pc"

.PHONY: all clean distclean install uninstall
