# Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
# Autoři:		Jakub Frýz (xfryzj01)

CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -g -std=c99

.PHONY: clean

all: main

main: *.c *.h
	$(CC) $(CFLAGS) *.c *.h -o main

debug: *.c *.h
	$(CC) $(CFLAGS) -Ddebug *.c *.h -o main

clean:
	rm -f *.zip main

zip: *.c *.h makefile rozdeleni docs/dokumentace.pdf
	zip --junk-paths --filesync xfryzj01.zip *.c *.h makefile rozdeleni docs/dokumentace.pdf
