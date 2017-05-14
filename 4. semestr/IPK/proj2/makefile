# IPK Projekt 2
# makefile

.PHONY: clean all

CXX=g++
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -pedantic

all: chat_client
dummy: chat_recv chat_send

chat_client: main.cpp
	$(CXX) $(CXXFLAGS) -pthread -o $@ $?

chat_info: main.cpp
	$(CXX) $(CXXFLAGS) -pthread -Dinfo -o $@ $?

chat_send: send.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?

chat_recv: recv.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?

clean:
	rm -f chat_client chat_test chat_recv chat_send chat_info *.tgz

pack: makefile main.cpp *.txt
	cp -f doc/ipk_chat.pdf ipk_chat.pdf
	tar pczvf xfryzj01.tgz $? *.pdf
