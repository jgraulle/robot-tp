BUILDDIR := build

.PHONY: all
all : $(BUILDDIR)/robot-git.pdf \
	$(BUILDDIR)/robot-tp1-simu.pdf \
	$(BUILDDIR)/robot-tp2-command-linetrack.pdf \
	$(BUILDDIR)/robot-tp2-command-linetrack.pdf \
	$(BUILDDIR)/robot-tp2-command-linetrack-simulator-docker-bin.tar.gz \
	$(BUILDDIR)/robot-tp2-command-linetrack-simulator-windows-bin.zip \
	$(BUILDDIR)/robot-tp3-embedded-adc.pdf \
	$(BUILDDIR)/robot-tp3-embedded-adc-header.tar.gz \
	$(BUILDDIR)/robot-tp4-embedded-linetrack.pdf \
	$(BUILDDIR)/robot-tp4-embedded-jsonrpctcpserver.tar.gz \
	$(BUILDDIR)/robot-tp5-embedded-rangefinder.pdf

$(BUILDDIR)/%.pdf : %.md | $(BUILDDIR)
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

$(BUILDDIR)/%.pdf : tp/%.md | $(BUILDDIR)
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

$(BUILDDIR)/%.tar.gz: tp/%.tar.gz | $(BUILDDIR)
	cp $^ $@

$(BUILDDIR)/%.zip: tp/%.zip | $(BUILDDIR)
	cp $^ $@

$(BUILDDIR)/robot-tp3-embedded-adc-header.tar.gz: tp/adc_cxx.hpp | $(BUILDDIR)
	tar -cvzf $@ -C tp $(subst tp/,,$^)

$(BUILDDIR)/robot-tp4-embedded-jsonrpctcpserver.tar.gz: tp/jsonrpctcpserver.hpp tp/jsonrpctcpserver.cpp | $(BUILDDIR)
	tar -cvzf $@ -C tp $(subst tp/,,$^)

$(BUILDDIR):
	mkdir $(BUILDDIR)
