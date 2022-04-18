#include "contenido_encriptado.hpp"
#include "contenido_desencriptado.hpp"
#include <string>

using std::string;

// argv[1] -> fichero .enc, argv[2] -> directorio volátil, argv[3] -> clave
int main(int argc, char *argv[]){
	
	// Extraemos el contenido encriptado
	ContenidoEncriptado contenido_encriptado(argv[1]);
	
	ContenidoDesencriptado contenido_desencriptado;
	string clave((const char *)argv[3]), directorio((const char *)argv[2]);
	
	// Establecemos la clave de desencriptado
	contenido_desencriptado.set_clave(clave);
	
	// Desencriptamos el contenido extraído
	contenido_desencriptado.desencriptar(contenido_encriptado.get_contenido(), contenido_encriptado.get_longitud());
	
	// Extraemos las entradas del fichero desencriptado
	contenido_desencriptado.extraer_entradas();
	
	//Creamos el content.xml
	contenido_desencriptado.generar_xml(directorio);

	return 0;
}
