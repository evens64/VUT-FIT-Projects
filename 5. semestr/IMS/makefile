## Soubor:         makefile
## Kódování:       UTF-8
## Projekt:        IMS projekt
## Autor:          Jakub Frýz          (xfryzj01)
##                 Filip Dostálík      (xdosta46)

.PHONY: clean pack

# compiling
COMPILER=g++
CFLAGS=-g -O2
PFLAGS=-lsimlib -lm

# zip name
YY=01
xlogin01=xfryzj01
xlogin02=xdosta46

# 'make <TARGET> TARGET=newmodel'
TARGET=model


all: $(TARGET)

%: %.cpp
	$(COMPILER) $(CFLAGS) -o $@ $? $(PFLAGS)

run:
	./$(TARGET)
	cat $(TARGET).out

clean:
	rm -rf $(TARGET) $(TARGET).out $(YY)_$(xlogin01)_$(xlogin02).zip

pack:
	zip --file-sync $(YY)_$(xlogin01)_$(xlogin02).zip $(TARGET).cpp $(TARGET).hpp makefile doc.pdf
