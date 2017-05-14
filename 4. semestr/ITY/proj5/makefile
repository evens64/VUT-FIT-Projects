# Vytvořil:			Jakub Frýz (xfryzj01)

.PHONY: clean latex dvips ps2pdf pdflatex

all: pdflatex

ID=xfryzj01
PROJEKT=proj5

latex: $(PROJEKT).tex
	latex $?
	latex $?

pdflatex: $(PROJEKT).tex
	pdflatex $?
	pdflatex $?

dvips: $(PROJEKT).dvi
	dvips -t a4 $?

ps2pdf: $(PROJEKT).ps
	ps2pdf $?

clean:
	rm -f *.zip *.pdf *.log *.aux *.dvi *.bak *.gz *.nav *.out *.snm *.toc
zip: $(PROJEKT).tex makefile img/*
	zip --filesync $(ID)-fit.zip $?
