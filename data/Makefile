TARGETS	= Asm.pc
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

Asm.pc: Asm.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "Asm.pc"

clean:
	$(RM) -- $(Asm.pc_OBJS)
	./pkgconfig.sh -c -P "$(PREFIX)" -- "Asm.pc"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "Asm.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "Asm.pc"

.PHONY: all clean distclean install uninstall
