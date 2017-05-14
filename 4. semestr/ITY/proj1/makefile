
# Vytvořil:			Jakub Frýz (xfryzj01)

.PHONY: clean

all: latex dvips ps2pdf

ID=xfryzj01

latex: proj1.tex
	latex $?

dvips: proj1.dvi
	dvips $?

ps2pdf: proj1.ps
	ps2pdf $?

clean:
	rm -f *.zip

zip: proj1.tex makefile
	zip --junk-paths --filesync $(ID)-fit.zip $?
