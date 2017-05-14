# ICP Projekt: makefile
# Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

.PHONY: run clean

a1=xhrabo08
a2=50
b1=xfryzj01
b2=50

zip_pack=src/* examples/* Doxyfile makefile README.txt
CXX=g++
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -pedantic

run: hra2017 hra2017-cli

hra2017:
#	command here

hra2017-cli: src/*.h src/*.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?

clean:
	rm -rf doc *.zip hra2017 hra2017-cli

doxygen:
	doxygen Doxyfile

pack:
	zip  --filesync $(a1)-$(b1)-$(a2)-$(b2).zip $(zip_pack)
