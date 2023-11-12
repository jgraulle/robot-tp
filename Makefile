.PHONY: all
all : build/robot-git.pdf build/robot-embedded-TP1.pdf build/robot-embedded-TP1-dac-adc-header.tar.gz build/robot-simu-TP1.pdf

build/%.pdf : %.md
	pandoc -V geometry:margin=2cm -o $@ -t latex $<

build/robot-embedded-TP1-dac-adc-header.tar.gz: adc_cxx.hpp dac_cxx.hpp
	tar -cvzf $@ $^
