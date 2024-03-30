BUILDDIR := build

.PHONY: all
all : $(BUILDDIR)/robot-git.pdf $(BUILDDIR)/robot-embedded-TP1.pdf $(BUILDDIR)/robot-embedded-TP1-adc-header.tar.gz $(BUILDDIR)/robot-simu-TP1.pdf \
	$(BUILDDIR)/robot-simu-TP2.pdf $(BUILDDIR)/robot-embedded-TP2.pdf \
	$(BUILDDIR)/robot-simu-TP3-PID.pdf $(BUILDDIR)/robot-embedded-TP3.pdf $(BUILDDIR)/robot-embedded-TP2-jsonrpctcpserver.tar.gz

$(BUILDDIR)/%.pdf : %.md | $(BUILDDIR)
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

$(BUILDDIR)/%.pdf : tp1/%.md | $(BUILDDIR)
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

$(BUILDDIR)/robot-embedded-TP1-adc-header.tar.gz: tp1/adc_cxx.hpp | $(BUILDDIR)
	tar -cvzf $@ -C tp1 $(subst tp1/,,$^)

$(BUILDDIR)/%.pdf : tp2-linetrack/%.md | $(BUILDDIR)
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

$(BUILDDIR)/%.pdf : tp3-pid/%.md | $(BUILDDIR)
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

$(BUILDDIR)/robot-embedded-TP2-jsonrpctcpserver.tar.gz: tp2-linetrack/jsonrpctcpserver.hpp tp2-linetrack/jsonrpctcpserver.cpp | $(BUILDDIR)
	tar -cvzf $@ -C tp2-linetrack $(subst tp2-linetrack/,,$^)

$(BUILDDIR):
	mkdir $(BUILDDIR)
