#ifndef __CONTENIDO_ENCRIPTADO_HPP
#define __CONTENIDO_ENCRIPTADO_HPP

#include <cstdint> //uint8_t
#include <string>
#include <list>
#include <fstream>
#include "entrada.hpp"

using std::string;
using std::fstream;
using std::ios_base;
using std::list;

class ContenidoEncriptado {

	private:
		unsigned char * _clave; // Clave de (des)encriptado
		uint8_t * _hash; // HASH de la clave de (des)encriptado
		uint8_t * _hash_nuclear; // HASH de la clave nuclear
		uint8_t * _contenido; // Contenido encritado
		size_t _contenido_length; // Longitud en bytes del contenido encriptado
		string _fichero; // Fichero que contiene la información encriptada
		
	public:
		//Constructor de la clase ContenidoEncriptado
		ContenidoEncriptado(const string);
	
		//Devuelve el hash de la clave de encriptado
		inline uint8_t * get_hash() const {return _hash;};
		
		//Devuelve el hash de la clave nuclear
		inline uint8_t * get_nuclear() const {return _hash_nuclear;};
		
		//Devuelve el contenido encriptado
		inline uint8_t * get_contenido() const {return _contenido;};
		
		// Devuelve la longitud en bytes del contenido encriptado dentro del fichero
		// que lo almacena
		inline size_t get_length_contenido() {
		
			std::streamsize tamanio = 0;
			
			fstream contenedor(_fichero, ios_base::in);
			std::streambuf * contenido = contenedor.rdbuf();
						
			// Obtenemos la longitud en bytes del contenido
			tamanio = (std::streamsize) contenido->pubseekoff(64, contenedor.end);
			
			return (size_t) tamanio;
		}
		
		//Establece el hash de la clave de encriptado
		inline void set_hash(const uint8_t * nueva_clave) {_hash = (uint8_t *)nueva_clave;};
		
		//Establece el hash de la clave nuclear
		inline void set_nuclear(const uint8_t * nueva_nuclear) {_hash_nuclear = (uint8_t *) nueva_nuclear;};
		
		//Establece el contenido encriptado
		inline void set_contenido(const uint8_t * nuevo_contenido) {_contenido = (uint8_t *) nuevo_contenido;};
		
		//Establece la clave de encriptado
		void set_clave(const string);
		
		// Extrae una serie de bytes del fichero que contiene tanto los hashes como
		// el contenido encriptado. El puntero devuelto es un puntero creado dinámicamente
		// por lo que deberá liberarse mediante delete [] una vez termine de usarse.
		uint8_t * extraer_bytes(const int inicio = 0, int n_bytes = -1) const;
		
		// Encripta la lista de entradas pasadas por parámetro y almacena el
		// resultado en _contenido
		bool encriptar_entradas(const list<Entrada> & entradas);
		
		bool volcar_contenido() const;
		
		//Destructor de la clase
		inline ~ContenidoEncriptado(){
		
			if(_hash != nullptr)
				delete [] _hash;
			
			if(_hash_nuclear != nullptr)
				delete [] _hash_nuclear;
			
			if(_contenido != nullptr)
				delete [] _contenido;
				
			if(_clave != nullptr)
				delete [] _clave;
		}
};

#endif
