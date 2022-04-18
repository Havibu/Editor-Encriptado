#include <iostream>
#include <string>
#include <iomanip>
#include "contenido_encriptado.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[]){

	try{
		ContenidoEncriptado contenido_encriptado(argv[1]);
		char * hash;
		
		// Obtenemos el hash de la clave
		hash = (char *)contenido_encriptado.get_hash();
		
		// Mostramos el hash de la clave. 
		// El doble casting es para evitar que los bytes que no correspondan a
		// caracteres imprimibles sean mostrados por pantalla antecedidos por ffffff.
		// Los métodos setfill() y setw() es para que los bytes de un solo caracter
		// (p.e. C, F, 4, etc.) se impriman con un 0 delante (p.e. 0C, 0F, 04, etc.)
		for(int i = 0; i < 32; i++)
			cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char) hash[i];
		
		cout << " ";
		
		// Obtenemos el hash de la clave nuclear
		hash = (char *)contenido_encriptado.get_nuclear();
		
		// Mostramos el hash de la clave nuclear por pantalla, de nuevo usando
		// el doble casting
		for(int i = 0; i < 32; i++)
			cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char) hash[i];
			
		cout << endl;
	}
	// El único error que puede ocurrir es que la usuario se le olvide indicar un fichero
	// que almacene el contenido que deseamos extraer
	catch(...){
		cout << "ERROR::HASHES -> ASEGURESE DE INDICAR UN FICHERO .enc" << endl;
		return -1;
	}
	
	return 0;
}
