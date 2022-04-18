.PHONY: clean
.PHONY: set

CC=g++
CFLAGS=-c -g -Wall -std=c++11 -lssl -lcrypto
OFLAGS=-o $@ $^ -std=c++11 -lssl -lcrypto

OBJECTS=fecha.o contenido_encriptado.o aes.o contenido_desencriptado.o hashes generar_content_xml actualizar_diario
CE_DEP=contenido_encriptado.hpp fecha.hpp entrada.hpp aes.hpp
CD_DEP=contenido_desencriptado.cpp contenido_desencriptado.hpp fecha.hpp entrada.hpp aes.hpp
XML_DEP=generar_content_xml.cpp fecha.hpp entrada.hpp aes.hpp contenido_encriptado.hpp contenido_desencriptado.hpp
ACT_DEP=actualizar_diario.cpp fecha.hpp entrada.hpp aes.hpp contenido_encriptado.hpp contenido_desencriptado.hpp

OBJ_HASH=hashes.o contenido_encriptado.o fecha.o entrada.hpp aes.o
OBJ_XML=generar_content_xml.o fecha.o entrada.hpp aes.o contenido_encriptado.o contenido_desencriptado.o
OBJ_ACT=actualizar_diario.o fecha.o entrada.hpp aes.o contenido_encriptado.o contenido_desencriptado.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) entrada.hpp

fecha.o: fecha.cpp fecha.hpp
	$(CC) $(CFLAGS) $^
	
aes.o: aes.cpp aes.hpp
	$(CC) $(CFLAGS) $^
	
contenido_encriptado.o: contenido_encriptado.cpp $(CE_DEP)
	$(CC) $(CFLAGS) $^

contenido_desencriptado.o: $(CD_DEP)
	$(CC) $(CFLAGS) $^

hashes: $(OBJ_HASH)
	$(CC) $(OFLAGS)
	
hashes.o: hashes.cpp $(CE_DEP)
	$(CC) $(CFLAGS) $^
	
generar_content_xml: $(OBJ_XML)
	$(CC) $(OFLAGS)
	
generar_content_xml.o: $(XML_DEP)
	$(CC) $(CFLAGS) $^
	
actualizar_diario: $(OBJ_ACT)
	$(CC) $(OFLAGS)
	
actualizar_diario.o: $(ACT_DEP)
	$(CC) $(CFLAGS) $^

clean:
	rm -rf *.o *.hpp.gch
