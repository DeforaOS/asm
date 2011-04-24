PACKAGE	= asm
VERSION	= 0.1.1
SUBDIRS	= data include src test
RM	= rm -f
LN	= ln -f
TAR	= tar -czvf


all: subdirs

subdirs:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE)) || exit; done

clean:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) clean) || exit; done

distclean:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) distclean) || exit; done

dist:
	$(RM) -r -- $(PACKAGE)-$(VERSION)
	$(LN) -s -- . $(PACKAGE)-$(VERSION)
	@$(TAR) $(PACKAGE)-$(VERSION).tar.gz -- \
		$(PACKAGE)-$(VERSION)/data/Makefile \
		$(PACKAGE)-$(VERSION)/data/asm.pc.in \
		$(PACKAGE)-$(VERSION)/data/pkgconfig.sh \
		$(PACKAGE)-$(VERSION)/data/project.conf \
		$(PACKAGE)-$(VERSION)/include/Asm.h \
		$(PACKAGE)-$(VERSION)/include/Makefile \
		$(PACKAGE)-$(VERSION)/include/project.conf \
		$(PACKAGE)-$(VERSION)/include/Asm/arch.h \
		$(PACKAGE)-$(VERSION)/include/Asm/asm.h \
		$(PACKAGE)-$(VERSION)/include/Asm/format.h \
		$(PACKAGE)-$(VERSION)/include/Asm/Makefile \
		$(PACKAGE)-$(VERSION)/include/Asm/project.conf \
		$(PACKAGE)-$(VERSION)/src/arch.c \
		$(PACKAGE)-$(VERSION)/src/asm.c \
		$(PACKAGE)-$(VERSION)/src/code.c \
		$(PACKAGE)-$(VERSION)/src/format.c \
		$(PACKAGE)-$(VERSION)/src/parser.c \
		$(PACKAGE)-$(VERSION)/src/token.c \
		$(PACKAGE)-$(VERSION)/src/main.c \
		$(PACKAGE)-$(VERSION)/src/deasm.c \
		$(PACKAGE)-$(VERSION)/src/Makefile \
		$(PACKAGE)-$(VERSION)/src/arch.h \
		$(PACKAGE)-$(VERSION)/src/asm.h \
		$(PACKAGE)-$(VERSION)/src/code.h \
		$(PACKAGE)-$(VERSION)/src/common.h \
		$(PACKAGE)-$(VERSION)/src/format.h \
		$(PACKAGE)-$(VERSION)/src/parser.h \
		$(PACKAGE)-$(VERSION)/src/token.h \
		$(PACKAGE)-$(VERSION)/src/project.conf \
		$(PACKAGE)-$(VERSION)/src/arch/amd64.c \
		$(PACKAGE)-$(VERSION)/src/arch/dalvik.c \
		$(PACKAGE)-$(VERSION)/src/arch/i386.c \
		$(PACKAGE)-$(VERSION)/src/arch/i386_real.c \
		$(PACKAGE)-$(VERSION)/src/arch/i486.c \
		$(PACKAGE)-$(VERSION)/src/arch/i586.c \
		$(PACKAGE)-$(VERSION)/src/arch/i686.c \
		$(PACKAGE)-$(VERSION)/src/arch/java.c \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.c \
		$(PACKAGE)-$(VERSION)/src/arch/sparc64.c \
		$(PACKAGE)-$(VERSION)/src/arch/Makefile \
		$(PACKAGE)-$(VERSION)/src/arch/amd64.ins \
		$(PACKAGE)-$(VERSION)/src/arch/amd64.reg \
		$(PACKAGE)-$(VERSION)/src/arch/common.ins \
		$(PACKAGE)-$(VERSION)/src/arch/dalvik.ins \
		$(PACKAGE)-$(VERSION)/src/arch/dalvik.reg \
		$(PACKAGE)-$(VERSION)/src/arch/i386.h \
		$(PACKAGE)-$(VERSION)/src/arch/i386.ins \
		$(PACKAGE)-$(VERSION)/src/arch/i386.reg \
		$(PACKAGE)-$(VERSION)/src/arch/i486.ins \
		$(PACKAGE)-$(VERSION)/src/arch/i586.ins \
		$(PACKAGE)-$(VERSION)/src/arch/i686.ins \
		$(PACKAGE)-$(VERSION)/src/arch/i686.reg \
		$(PACKAGE)-$(VERSION)/src/arch/null.ins \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.h \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.ins \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.reg \
		$(PACKAGE)-$(VERSION)/src/arch/project.conf \
		$(PACKAGE)-$(VERSION)/src/format/dex.c \
		$(PACKAGE)-$(VERSION)/src/format/elf.c \
		$(PACKAGE)-$(VERSION)/src/format/flat.c \
		$(PACKAGE)-$(VERSION)/src/format/java.c \
		$(PACKAGE)-$(VERSION)/src/format/pe.c \
		$(PACKAGE)-$(VERSION)/src/format/Makefile \
		$(PACKAGE)-$(VERSION)/src/format/project.conf \
		$(PACKAGE)-$(VERSION)/test/amd64.S \
		$(PACKAGE)-$(VERSION)/test/dalvik.S \
		$(PACKAGE)-$(VERSION)/test/i386.S \
		$(PACKAGE)-$(VERSION)/test/i386_real.S \
		$(PACKAGE)-$(VERSION)/test/i486.S \
		$(PACKAGE)-$(VERSION)/test/i586.S \
		$(PACKAGE)-$(VERSION)/test/i686.S \
		$(PACKAGE)-$(VERSION)/test/java.S \
		$(PACKAGE)-$(VERSION)/test/sparc.S \
		$(PACKAGE)-$(VERSION)/test/sparc64.S \
		$(PACKAGE)-$(VERSION)/test/Makefile \
		$(PACKAGE)-$(VERSION)/test/project.conf \
		$(PACKAGE)-$(VERSION)/Makefile \
		$(PACKAGE)-$(VERSION)/COPYING \
		$(PACKAGE)-$(VERSION)/config.h \
		$(PACKAGE)-$(VERSION)/config.sh \
		$(PACKAGE)-$(VERSION)/project.conf
	$(RM) -- $(PACKAGE)-$(VERSION)

install:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) install) || exit; done

uninstall:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) uninstall) || exit; done

.PHONY: all subdirs clean distclean dist install uninstall
