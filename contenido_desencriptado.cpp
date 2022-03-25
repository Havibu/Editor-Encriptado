#include "contenido_desencriptado.hpp"

void ContenidoDesencriptado::set_clave(const string clave) {

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

void ContenidoDesencriptado::set_clave(const unsigned char *clave) {
	
	for(int i = 0; i < 32; i++)
		_clave[i] = clave[i];
}

bool ContenidoDesencriptado::desencriptar(const uint8_t * contenido_encriptado, const size_t tamanio) {

	secure_string contenido_cifrado(tamanio, 'a'); // Texto a desencriptar
	secure_string contenido_descifrado; //Texto recuperado
	
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
				
	// Convertimos el contenido a desencriptar a secure string
	for(size_t i = 0; i < tamanio; i++)
		contenido_cifrado[i] = contenido_encriptado[i];
	
	try {
		// Cargamos el método de cifrado AES CBC 256
	    	EVP_add_cipher(EVP_aes_256_cbc());
		
		// Desencriptamos el contenido
		aes_decrypt(clave, iv, contenido_cifrado, contenido_descifrado);
		
		// Reservamos memoria para almacenar el contenido desencriptado
		_longitud = contenido_descifrado.length();
		_contenido_desencriptado = new unsigned char[_longitud];
		
		// Almacenamos el contenido desencriptado
		for(long unsigned int i = 0; i < _longitud; i++)
			_contenido_desencriptado[i] = contenido_descifrado[i];
	}
	catch(...){
		return false;
	}
	
	return true;
}

bool ContenidoDesencriptado::extraer_entradas() {
		
	size_t n_entradas = 0;
	
	// Obtenemos el número de entradas a extraer
	for(size_t i = 0; i < _longitud; i++)
		if(_contenido_desencriptado[i] == '\222') n_entradas++;
	
	//TODO: TERMINAR
	
	return true;
}







