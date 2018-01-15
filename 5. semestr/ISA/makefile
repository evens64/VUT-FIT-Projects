# Soubor:		makefile
# Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)
# Projekt:		LDAP server

.PHONY: clean pack

# variables
LOGIN=xfryzj01
CXX=g++
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -pedantic

# default target
all: prepare myldap

# executable
myldap: $(addsuffix .o, $(basename $(notdir $(wildcard src/*.cpp))))
	$(CXX) $(CXXFLAGS) -o $@ $(addprefix obj/, $?)

# obj files
%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o obj/$@ $?

# other
clean:
	rm -rf obj myldap $(LOGIN).zip

pack:
	zip --filesync $(LOGIN).zip src/* doc/dokumentace.pdf makefile db.csv readme.txt

prepare:
	mkdir -p obj
