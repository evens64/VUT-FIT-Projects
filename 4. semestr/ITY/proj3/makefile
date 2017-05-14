# Vytvořil:			Jakub Frýz (xfryzj01)

.PHONY: clean latex dvips ps2pdf

all: latex dvips ps2pdf

ID=xfryzj01
PROJEKT=proj3

latex: $(PROJEKT).tex
	echo -e "\033[1;32mLATEX 1:\033[0m"
	latex $?
	echo -e "\033[1;32mLATEX 2:\033[0m"
	latex $?

dvips: $(PROJEKT).dvi
	echo -e "\033[1;32mDVIPS:\033[0m"
	dvips -t a4 $?

ps2pdf: $(PROJEKT).ps
	echo -e "\033[1;32mPS2PDF:\033[0m"
	ps2pdf $?

clean:
	rm -f *.zip *.pdf *.log *.aux *.dvi *.bak *.gz include/*.pdf

zip: $(PROJEKT).tex makefile include/*.eps
	zip --filesync $(ID)-fit.zip $?
