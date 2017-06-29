# ICP Projekt: makefile
# Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

.PHONY: run clean

a1=xhrabo08
a2=50
b1=xfryzj01
b2=50

zip_pack=src/* src/img/* examples/* Doxyfile makefile README.txt
cmd_game=src/Card.cpp src/Card.h src/CardStack.cpp src/CardStack.h src/ConsoleSolitaireBoard.cpp src/ConsoleSolitaireBoard.h src/ConsoleSolitaireGame.cpp src/ConsoleSolitaireGame.h src/SolitaireBoard.cpp src/SolitaireBoard.h src/main_cli.cpp

CXX=g++
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -pedantic

run: hra2017-cli hra2017

hra2017:
	qmake src/hra2017.pro -spec linux-g++
	cd build && $(MAKE) qmake_all

hra2017-cli: $(cmd_game)
	$(CXX) $(CXXFLAGS) -o build/$@ $?

clean:
	rm -rf doc build *.zip

doxygen:
	doxygen Doxyfile

pack:
	zip  --filesync $(a1)-$(b1)-$(a2)-$(b2).zip $(zip_pack)
