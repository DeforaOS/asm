TARGETS	= $(OBJDIR)Asm.pc
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)Asm.pc: Asm.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "$(OBJDIR)Asm.pc"

clean:
	$(RM) -- $(Asm.pc_OBJS)
	./pkgconfig.sh -c -P "$(PREFIX)" -- "Asm.pc"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)Asm.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "Asm.pc"

.PHONY: all clean distclean install uninstall
