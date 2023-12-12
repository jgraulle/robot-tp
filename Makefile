.PHONY: all
all : build/robot-git.pdf build/robot-embedded-TP1.pdf build/robot-embedded-TP1-dac-adc-header.tar.gz build/robot-simu-TP1.pdf \
	build/robot-simu-TP2.pdf build/robot-embedded-TP2.pdf

build/%.pdf : %.md
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

build/%.pdf : tp1/%.md
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

build/robot-embedded-TP1-dac-adc-header.tar.gz: tp1/adc_cxx.hpp tp1/dac_cxx.hpp
	tar -cvzf $@ $^

build/%.pdf : tp2-linetrack/%.md
	pandoc -V geometry:margin=2cm -o $@ -t latex $<
