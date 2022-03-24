.PHONY: clean

CC=g++
CFLAGS=-c -g -Wall -std=c++11 -lssl -lcrypto

OBJECTS=fecha.o contenido_encriptado.o aes.o
CE_DEP=contenido_encriptado.cpp contenido_encriptado.hpp fecha.hpp entrada.hpp aes.hpp

all: $(OBJECTS)
	$(CC) $(CFLAGS) entrada.hpp

fecha.o: fecha.cpp fecha.hpp
	$(CC) $(CFLAGS) $^
	
aes.o: aes.cpp aes.hpp
	$(CC) $(CFLAGS) $^
	
contenido_encriptado.o: $(CE_DEP)
	$(CC) $(CFLAGS) $^
	
clean:
	rm -rf *.o *.hpp.gch
