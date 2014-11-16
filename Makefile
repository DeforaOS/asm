PACKAGE	= Asm
VERSION	= 0.2.1
SUBDIRS	= data doc include src tools tests
RM	= rm -f
LN	= ln -f
TAR	= tar
MKDIR	= mkdir -m 0755 -p


all: subdirs

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE)) || exit; done

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) clean) || exit; done

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) distclean) || exit; done

dist:
	$(RM) -r -- $(PACKAGE)-$(VERSION)
	$(LN) -s -- . $(PACKAGE)-$(VERSION)
	@$(TAR) -czvf $(OBJDIR)$(PACKAGE)-$(VERSION).tar.gz -- \
		$(PACKAGE)-$(VERSION)/data/Makefile \
		$(PACKAGE)-$(VERSION)/data/Asm.pc.in \
		$(PACKAGE)-$(VERSION)/data/pkgconfig.sh \
		$(PACKAGE)-$(VERSION)/data/project.conf \
		$(PACKAGE)-$(VERSION)/doc/Makefile \
		$(PACKAGE)-$(VERSION)/doc/GRAMMAR \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc.sh \
		$(PACKAGE)-$(VERSION)/doc/project.conf \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/Asm-docs.xml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/project.conf \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/Asm-unused.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/Asm.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/arch.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/asm.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/code.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/common.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/format.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/project.conf \
		$(PACKAGE)-$(VERSION)/include/Asm.h \
		$(PACKAGE)-$(VERSION)/include/Makefile \
		$(PACKAGE)-$(VERSION)/include/project.conf \
		$(PACKAGE)-$(VERSION)/include/Asm/arch.h \
		$(PACKAGE)-$(VERSION)/include/Asm/asm.h \
		$(PACKAGE)-$(VERSION)/include/Asm/code.h \
		$(PACKAGE)-$(VERSION)/include/Asm/common.h \
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
		$(PACKAGE)-$(VERSION)/src/code.h \
		$(PACKAGE)-$(VERSION)/src/common.h \
		$(PACKAGE)-$(VERSION)/src/format.h \
		$(PACKAGE)-$(VERSION)/src/parser.h \
		$(PACKAGE)-$(VERSION)/src/token.h \
		$(PACKAGE)-$(VERSION)/src/python/project.conf \
		$(PACKAGE)-$(VERSION)/src/python/Makefile \
		$(PACKAGE)-$(VERSION)/src/python/libasm.c \
		$(PACKAGE)-$(VERSION)/src/python/libasm.py \
		$(PACKAGE)-$(VERSION)/src/project.conf \
		$(PACKAGE)-$(VERSION)/src/arch/amd64.c \
		$(PACKAGE)-$(VERSION)/src/arch/arm.c \
		$(PACKAGE)-$(VERSION)/src/arch/armeb.c \
		$(PACKAGE)-$(VERSION)/src/arch/armel.c \
		$(PACKAGE)-$(VERSION)/src/arch/dalvik.c \
		$(PACKAGE)-$(VERSION)/src/arch/i386.c \
		$(PACKAGE)-$(VERSION)/src/arch/i386_real.c \
		$(PACKAGE)-$(VERSION)/src/arch/i486.c \
		$(PACKAGE)-$(VERSION)/src/arch/i586.c \
		$(PACKAGE)-$(VERSION)/src/arch/i686.c \
		$(PACKAGE)-$(VERSION)/src/arch/java.c \
		$(PACKAGE)-$(VERSION)/src/arch/mips.c \
		$(PACKAGE)-$(VERSION)/src/arch/mipseb.c \
		$(PACKAGE)-$(VERSION)/src/arch/mipsel.c \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.c \
		$(PACKAGE)-$(VERSION)/src/arch/sparc64.c \
		$(PACKAGE)-$(VERSION)/src/arch/template.c \
		$(PACKAGE)-$(VERSION)/src/arch/yasep.c \
		$(PACKAGE)-$(VERSION)/src/arch/yasep16.c \
		$(PACKAGE)-$(VERSION)/src/arch/yasep32.c \
		$(PACKAGE)-$(VERSION)/src/arch/Makefile \
		$(PACKAGE)-$(VERSION)/src/arch/amd64.ins \
		$(PACKAGE)-$(VERSION)/src/arch/amd64.reg \
		$(PACKAGE)-$(VERSION)/src/arch/arm.h \
		$(PACKAGE)-$(VERSION)/src/arch/arm.ins \
		$(PACKAGE)-$(VERSION)/src/arch/arm.reg \
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
		$(PACKAGE)-$(VERSION)/src/arch/mips.h \
		$(PACKAGE)-$(VERSION)/src/arch/mips.ins \
		$(PACKAGE)-$(VERSION)/src/arch/mips.reg \
		$(PACKAGE)-$(VERSION)/src/arch/null.ins \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.h \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.ins \
		$(PACKAGE)-$(VERSION)/src/arch/sparc.reg \
		$(PACKAGE)-$(VERSION)/src/arch/yasep.h \
		$(PACKAGE)-$(VERSION)/src/arch/yasep.ins \
		$(PACKAGE)-$(VERSION)/src/arch/yasep.reg \
		$(PACKAGE)-$(VERSION)/src/arch/project.conf \
		$(PACKAGE)-$(VERSION)/src/format/dex.c \
		$(PACKAGE)-$(VERSION)/src/format/elf.c \
		$(PACKAGE)-$(VERSION)/src/format/flat.c \
		$(PACKAGE)-$(VERSION)/src/format/java.c \
		$(PACKAGE)-$(VERSION)/src/format/pe.c \
		$(PACKAGE)-$(VERSION)/src/format/Makefile \
		$(PACKAGE)-$(VERSION)/src/format/project.conf \
		$(PACKAGE)-$(VERSION)/tools/arch.c \
		$(PACKAGE)-$(VERSION)/tools/arch/amd64.c \
		$(PACKAGE)-$(VERSION)/tools/arch/arm.c \
		$(PACKAGE)-$(VERSION)/tools/arch/armeb.c \
		$(PACKAGE)-$(VERSION)/tools/arch/armel.c \
		$(PACKAGE)-$(VERSION)/tools/arch/dalvik.c \
		$(PACKAGE)-$(VERSION)/tools/arch/i386.c \
		$(PACKAGE)-$(VERSION)/tools/arch/i386_real.c \
		$(PACKAGE)-$(VERSION)/tools/arch/i486.c \
		$(PACKAGE)-$(VERSION)/tools/arch/i586.c \
		$(PACKAGE)-$(VERSION)/tools/arch/i686.c \
		$(PACKAGE)-$(VERSION)/tools/arch/java.c \
		$(PACKAGE)-$(VERSION)/tools/arch/mips.c \
		$(PACKAGE)-$(VERSION)/tools/arch/mipseb.c \
		$(PACKAGE)-$(VERSION)/tools/arch/mipsel.c \
		$(PACKAGE)-$(VERSION)/tools/arch/sparc.c \
		$(PACKAGE)-$(VERSION)/tools/arch/sparc64.c \
		$(PACKAGE)-$(VERSION)/tools/arch/yasep.c \
		$(PACKAGE)-$(VERSION)/tools/arch/yasep16.c \
		$(PACKAGE)-$(VERSION)/tools/arch/yasep32.c \
		$(PACKAGE)-$(VERSION)/tools/format.c \
		$(PACKAGE)-$(VERSION)/tools/format/dex.c \
		$(PACKAGE)-$(VERSION)/tools/format/elf.c \
		$(PACKAGE)-$(VERSION)/tools/format/flat.c \
		$(PACKAGE)-$(VERSION)/tools/format/java.c \
		$(PACKAGE)-$(VERSION)/tools/format/pe.c \
		$(PACKAGE)-$(VERSION)/tools/Makefile \
		$(PACKAGE)-$(VERSION)/tools/project.conf \
		$(PACKAGE)-$(VERSION)/tests/amd64.asm \
		$(PACKAGE)-$(VERSION)/tests/arm.asm \
		$(PACKAGE)-$(VERSION)/tests/armeb.asm \
		$(PACKAGE)-$(VERSION)/tests/armel.asm \
		$(PACKAGE)-$(VERSION)/tests/dalvik.asm \
		$(PACKAGE)-$(VERSION)/tests/i386.asm \
		$(PACKAGE)-$(VERSION)/tests/i386_real.asm \
		$(PACKAGE)-$(VERSION)/tests/i486.asm \
		$(PACKAGE)-$(VERSION)/tests/i586.asm \
		$(PACKAGE)-$(VERSION)/tests/i686.asm \
		$(PACKAGE)-$(VERSION)/tests/mips.asm \
		$(PACKAGE)-$(VERSION)/tests/mipseb.asm \
		$(PACKAGE)-$(VERSION)/tests/mipsel.asm \
		$(PACKAGE)-$(VERSION)/tests/java.asm \
		$(PACKAGE)-$(VERSION)/tests/sparc.asm \
		$(PACKAGE)-$(VERSION)/tests/sparc64.asm \
		$(PACKAGE)-$(VERSION)/tests/template.asm \
		$(PACKAGE)-$(VERSION)/tests/yasep.asm \
		$(PACKAGE)-$(VERSION)/tests/yasep16.asm \
		$(PACKAGE)-$(VERSION)/tests/yasep32.asm \
		$(PACKAGE)-$(VERSION)/tests/Makefile \
		$(PACKAGE)-$(VERSION)/tests/pylint.sh \
		$(PACKAGE)-$(VERSION)/tests/python.sh \
		$(PACKAGE)-$(VERSION)/tests/tests.sh \
		$(PACKAGE)-$(VERSION)/tests/project.conf \
		$(PACKAGE)-$(VERSION)/Makefile \
		$(PACKAGE)-$(VERSION)/COPYING \
		$(PACKAGE)-$(VERSION)/config.h \
		$(PACKAGE)-$(VERSION)/config.sh \
		$(PACKAGE)-$(VERSION)/project.conf
	$(RM) -- $(PACKAGE)-$(VERSION)

distcheck: dist
	$(TAR) -xzvf $(PACKAGE)-$(VERSION).tar.gz
	$(MKDIR) -- $(PACKAGE)-$(VERSION)/objdir
	$(MKDIR) -- $(PACKAGE)-$(VERSION)/destdir
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/")
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" DESTDIR="$$PWD/destdir" install)
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" DESTDIR="$$PWD/destdir" uninstall)
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" distclean)
	(cd "$(PACKAGE)-$(VERSION)" && $(MAKE) dist)
	$(RM) -r -- $(PACKAGE)-$(VERSION)

install:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) install) || exit; done

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) uninstall) || exit; done

.PHONY: all subdirs clean distclean dist distcheck install uninstall
