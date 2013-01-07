PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install
INCLUDEDIR= $(PREFIX)/include


all:

clean:

distclean: clean

install:
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Devel/Asm
	$(INSTALL) -m 0644 -- arch.h $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/arch.h
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Devel/Asm
	$(INSTALL) -m 0644 -- asm.h $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/asm.h
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Devel/Asm
	$(INSTALL) -m 0644 -- code.h $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/code.h
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Devel/Asm
	$(INSTALL) -m 0644 -- common.h $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/common.h
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Devel/Asm
	$(INSTALL) -m 0644 -- format.h $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/format.h

uninstall:
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/arch.h
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/asm.h
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/code.h
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/common.h
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Devel/Asm/format.h

.PHONY: all clean distclean install uninstall
