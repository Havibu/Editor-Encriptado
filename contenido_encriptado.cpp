#include "aes.hpp"
#include "contenido_encriptado.hpp"
#include "fecha.hpp"
#include <sstream> //stringstream

ContenidoEncriptado::ContenidoEncriptado(const string fichero):
_fichero(fichero) {

	//Extraemos el hash de la clave de cifrado
	_hash = this->extraer_bytes(0, 32);
	
	//Extraemos el hash de la clave nuclear
	_hash_nuclear = this->extraer_bytes(32, 32);
	
	//Extraemos el contenido encriptado
	_contenido = this->extraer_bytes(64);
	
	//Inicializamos la clave (mas que nada para el destructor de la clase)
	_clave = nullptr;
}

void ContenidoEncriptado::set_clave(const string clave) {

	// Dado que estamos usando el algoritmo de cifrado AES 256, la clave de cifrado
	// será de 256 bits, o lo que es lo mismo 32 bytes, es decir 32 caracteres. Si
	// la clave introducida por el usuario es menor de 32 caracteres se usará este
	// array de caracteres pseudoaleatorios para rellenar la clave de forma controlada
	const char relleno[32] = {'%', '/', '!', '$', '&', '?', '@', 's', '#', 'X', '_', '<',
				   'f', 'Y', '{', '>', 'P', 'a', '=', 'k', '$', '^', '|', '+',
				   'C', ',', ')', 'd', '.', '[', 'l', 'Q'};
				   
	// Inicializamos la clave para que sea igual al relleno
	string clave_32(relleno);
	
	// Generamos una clave a partir de la clave pasada por parámetro y el relleno
	// de forma que vaya alternando entre la clave y el relleno.
	for(long unsigned int i = 0, j = 0; j < clave.length() && i < 32; i++) {
	
		if(i % 2 == 0)
			clave_32[i] = clave[j];
		else {
			clave_32[i] = relleno[j];
			j++;
		}
	}
	
	// Reservamos memoria para la almacenar la clave generada
	_clave = new unsigned char[32];	
	
	// Almacenamos la clave generada
	for(int i = 0; i < 32; i++)
		_clave[i] = clave_32[i];
}

void ContenidoEncriptado::set_clave(const unsigned char *clave) {
	
	for(int i = 0; i < 32; i++)
		_clave[i] = clave[i];
}

uint8_t * ContenidoEncriptado::extraer_bytes(const int inicio, int nbytes) {

	fstream contenedor(_fichero, ios_base::in);
	std::streambuf * contenedor_buffer;
	std::streamsize contenedor_nbytes;
	uint8_t * contenido;
	
	// Éxito al abrir
	if(contenedor.is_open()) {
	
		//Extraemos el buffer del fichero para leer del mismo
		contenedor_buffer = contenedor.rdbuf();
		
		// Si el usuario especifica el número de bytes a extraer, esta variable 
		// tomará ese valor pero si el usuario no especifica ningún valor
		// específico esta valdrá tanto como bytes haya en el fichero desde la 
		// posición indicada hasta el final del fichero
		contenedor_nbytes = (nbytes != -1) ? (std::streamsize) nbytes : (std::streamsize) contenedor_buffer->pubseekoff(inicio, contenedor.end);
		
		// Reservamos dinámicamente la cantidad de memoria que vamos a necesitar para
		// almacenar el contenido extraído
		contenido = new uint8_t[contenedor_nbytes];
		
		//Retrocedemos a la posición indicada por el usuario
		contenedor_buffer->pubseekoff(inicio, contenedor.beg);
		
		// Extraemos la cantidad de información solicitada y la almacenamos
		// en "contenido". La razón de que almacenemos el número de caracteres
		// copiados cada vez que se llame a esta función es con visión de que la última
		// vez que se llamará a esta función será para extraer el contenido encriptado
		// del fichero. Es por eso que se almacena en la variable miembro asociada a
		// la longitud de dicho contenido.
		_contenido_length = contenedor_buffer->sgetn( (char *) contenido, contenedor_nbytes);
		
		//Cerramos el fichero
		contenedor.close();
	}
	else{
		cerr << "ERROR::CONTENIDO_ENCRIPTADO::EXTRAER_BYTES" << endl;
		contenido = nullptr;
	}
	
	return contenido;
}

//byte entre datos de fecha: \220
//byte entre fecha y contenido: \221
//byte entre entradas: \222
bool ContenidoEncriptado::encriptar_entradas(const list<Entrada> & entradas) {

	list<Entrada>::const_iterator entrada;
	string contenido_a_encriptar;
	Fecha fecha;
	std::stringstream contenido_ss;
	
	secure_string contenido_secure; // Texto a encriptar
	secure_string contenido_cifrado; //Texto cifrado
	
	//Creamos la clave de encriptado
	byte clave[KEY_SIZE];
	
	//Obtenemos la clave
	for(int i = 0; i < (int) KEY_SIZE; i++)
		clave[i] = (byte) _clave[i];
	
	// Parámetro de AES creado mediante un generador pseudoaleatorio 
	// criptográficamente seguro (OpenSSL)
	byte iv[BLOCK_SIZE] = {(byte)'<', (byte)'{', (byte)'^', (byte)'\341', (byte)'b',
				(byte)'\226', (byte)'\345', (byte)'\"', (byte)'|',
				(byte)'\375', (byte)'\r', (byte)'\325', (byte)'7',
				(byte)'4', (byte)'*', (byte)'\220'};
	
	try {
		// Metemos el contenido a encriptar en un string stream
		for(entrada = entradas.begin(); entrada != entradas.end(); entrada++){
		
			fecha = entrada->get_fecha();
			
			if(fecha.get_dia() < 10) contenido_ss << '0';
			contenido_ss << fecha.get_dia() << '\220';
			
			if(fecha.get_mes() < 10) contenido_ss << '0';
			contenido_ss << fecha.get_mes() << '\220' << fecha.get_anio() << '\221' 
				     << entrada->get_contenido() << '\222';
			
		}
		
		// Cargamos el método de cifrado AES CBC 256
	    	EVP_add_cipher(EVP_aes_256_cbc());
		
		// Convertimos el contenido a encriptar en un 'secure_string'
		contenido_a_encriptar = contenido_ss.str();
		contenido_secure = (const char *) contenido_a_encriptar.c_str();
		
		// Encriptamos el contenido
		aes_encrypt(clave, iv, contenido_secure, contenido_cifrado);
		
		// Reservamos la memoria necesaria
		this->_contenido = new uint8_t[contenido_cifrado.length()];
		
		// Almacenamos el contenido cifrado en _contenido
		for(long unsigned int i = 0; i < contenido_cifrado.length(); i++)
			this->_contenido[i] = contenido_cifrado[i];
			
		// Actualizamos la longitud del contenido
		_contenido_length = contenido_cifrado.length();
	}
	
	// En caso de que ocurra algún error
	catch(...){
		cerr << "ERROR::CONTENIDO_ENCRIPTADO::ENCRIPTAR_ENTRADAS" << endl;
		return false;
	}
	
	return true;
}

bool ContenidoEncriptado::volcar_contenido() const {
	
	try {
		// Abrimos el fichero de volcado en modo escritura
		fstream contenedor(_fichero, ios_base::out);
		std::streambuf * contenido = contenedor.rdbuf();
		
		// Escribimos el hash de la clave en el documento
		contenido->sputn( (const char *) _hash, 32);
		
		// Escribimos el hash de la clave nuclear en el documento
		contenido->sputn( (const char *) _hash_nuclear, 32);
		
		// Escribimos el contenido encriptado en el documento
		contenido->sputn( (const char *) _contenido, _contenido_length);
		
		// Cerramos el fichero para guardar los cambios en disco
		contenedor.close();
	}
	
	catch(...){
		cerr << "ERROR::CONTENIDO_ENCRIPTADO::VOLCAR_CONTENIDO" << endl;
		return false;
	}
	
	return true;
}







