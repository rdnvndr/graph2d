PACKAGE  = graph2d
SOURCES  = $(shell grep -l '\<_\>' ../src/*.C)
POFILES  = $(wildcard *.po)
POXFILES = $(POFILES:.po=.pox)
MOFILES  = $(POFILES:.po=.mo)

XGETTEXT =	xgettext
MSGMERGE =	msgmerge --indent --verbose
MSGFMT   =	msgfmt --check --statistics 

.SUFFIXES:
.SUFFIXES: .po .mo

all: $(MOFILES)

install: all
	@echo Installing message catalogues in $(LOCDIR)
	@for c in $(MOFILES); do \
	  LANG=`echo $$c|sed -e 's/.mo//'` ; \
	  MSGDIR="$(LOCDIR)/$$LANG/LC_MESSAGES"; \
	  echo Installing language: $$LANG ; \
	  $(INSTALLDIR) $$MSGDIR; \
	  $(INSTALLLIB) $$c $$MSGDIR/$(PACKAGE).mo; \
	done

$(PACKAGE).pot: POTFILES.in
	$(XGETTEXT) --default-domain=$(PACKAGE) --directory=../src \
	--add-comments --keyword=_ --keyword=N_ --msgstr-prefix="" \
	--msgstr-suffix="" --files-from=POTFILES.in

clean:
	rm -f  $(MOFILES) *~


merge: $(POXFILES)

%.pox: %.po $(PACKAGE).pot
	$(MSGMERGE) --indent --output-file $@ $< $(PACKAGE).pot

# convert portable into machine objects
.po.mo:
	$(MSGFMT) -o $@ $<