# Vytvořil:			Jakub Frýz (xfryzj01)

.PHONY: clean jf_latex jf_dvips jf_ps2pdf jf_bibtex projekt

all: projekt

ID=xfryzj01
PROJEKT=proj4

projekt:
	clear
	make jf_latex
	make jf_bibtex
	make jf_latex
	make jf_latex
	make jf_dvips
	make jf_ps2pdf


jf_latex: $(PROJEKT).tex
	latex $?

jf_bibtex: $(PROJEKT).aux
	bibtex $?

jf_dvips: $(PROJEKT).dvi
	dvips -t a4 $?

jf_ps2pdf: $(PROJEKT).ps
	ps2pdf $?


clean:
	rm -f *.zip *.pdf *.log *.aux *.dvi *.bak *.gz *.bbl *.blg *.ps *.out
	clear

pack: $(PROJEKT).tex $(PROJEKT).bib *.bst makefile
	clear
	zip --junk-paths --filesync $(ID)-fit.zip $?
