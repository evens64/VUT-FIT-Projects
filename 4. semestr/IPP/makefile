
# Vytvořil:			Jakub Frýz (xfryzj01)

.PHONY: clean

all: jsn mka

ID=xfryzj01

clean:
	rm -f *.zip

jsn: proj1/*.php tex/JSN-doc.pdf
	zip --junk-paths --filesync $(ID)-JSN.zip $?

mka: proj2/*.py tex/MKA-doc.pdf
	zip --junk-paths --filesync $(ID)-MKA.zip $?
