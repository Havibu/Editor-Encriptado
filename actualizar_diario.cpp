#include "contenido_desencriptado.hpp"
#include "contenido_encriptado.hpp"
#include <string>

using std::string;

int main(int argc, char *argv[]){
	
	ContenidoDesencriptado contenido_desencriptado;
	ContenidoEncriptado contenido_encriptado((const char*)argv[2]);
	string clave((const char*) argv[3]);
	
	// Cargamos las entradas del content.xml
	contenido_desencriptado.actualizar_entradas((const char*)argv[1]);
	
	// Establecemos la clave de encriptado
	contenido_encriptado.set_clave(clave);
	
	// Encriptamos las entradas
	contenido_encriptado.encriptar_entradas(contenido_desencriptado.get_entradas());
	
	// Volcamos el contenido
	contenido_encriptado.volcar_contenido();
	
	return 0;
}
