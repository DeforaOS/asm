PREFIX	= /usr/local
DESTDIR	=
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install
INCLUDEDIR= $(PREFIX)/include


all:

clean:

distclean: clean

install:
	$(MKDIR) $(DESTDIR)$(PREFIX)/include/Devel/Asm
	$(INSTALL) -m 0644 -- arch.h $(DESTDIR)$(PREFIX)/include/Devel/Asm/arch.h
	$(MKDIR) $(DESTDIR)$(PREFIX)/include/Devel/Asm
	$(INSTALL) -m 0644 -- asm.h $(DESTDIR)$(PREFIX)/include/Devel/Asm/asm.h
	$(MKDIR) $(DESTDIR)$(PREFIX)/include/Devel/Asm
	$(INSTALL) -m 0644 -- common.h $(DESTDIR)$(PREFIX)/include/Devel/Asm/common.h
	$(MKDIR) $(DESTDIR)$(PREFIX)/include/Devel/Asm
	$(INSTALL) -m 0644 -- format.h $(DESTDIR)$(PREFIX)/include/Devel/Asm/format.h

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/include/Devel/Asm/arch.h
	$(RM) -- $(DESTDIR)$(PREFIX)/include/Devel/Asm/asm.h
	$(RM) -- $(DESTDIR)$(PREFIX)/include/Devel/Asm/common.h
	$(RM) -- $(DESTDIR)$(PREFIX)/include/Devel/Asm/format.h

.PHONY: all clean distclean install uninstall
