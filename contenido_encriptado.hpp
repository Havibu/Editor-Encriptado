#ifndef __CONTENIDO_ENCRIPTADO_HPP
#define __CONTENIDO_ENCRIPTADO_HPP

#include <cstdint> //uint8_t
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include "entrada.hpp"

using std::string;
using std::fstream;
using std::ios_base;
using std::list;
using std::cerr;
using std::endl;

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
		
		// Devuelve la longitud del contenido almacenado
		inline size_t get_longitud() const {return _contenido_length;};
		
		//Establece el hash de la clave de encriptado
		inline void set_hash(const uint8_t * nueva_clave) {
			for(int i = 0; i < 32; i++)
				_hash[i] = nueva_clave[i];
		};
		
		//Establece el hash de la clave nuclear
		inline void set_nuclear(const uint8_t * nueva_nuclear) {
			for(int i = 0; i < 32; i++)
				_hash_nuclear[i] = nueva_nuclear[i];
		};
		
		//Establece el contenido encriptado
		inline void set_contenido(const uint8_t * nuevo_contenido) {_contenido = (uint8_t *)nuevo_contenido;};
				
		//Establece la clave de encriptado
		void set_clave(const string);

		void set_clave (const unsigned char *);
		
		// Extrae una serie de bytes del fichero que contiene tanto los hashes como
		// el contenido encriptado. El puntero devuelto es un puntero creado dinámicamente
		// por lo que deberá liberarse mediante delete [] una vez termine de usarse.
		uint8_t * extraer_bytes(const int inicio = 0, int n_bytes = -1);
		
		// Encripta la lista de entradas pasadas por parámetro y almacena el
		// resultado en _contenido
		bool encriptar_entradas(const list<Entrada> & entradas);
		
		// Volcamos los hashes, y el contenido encriptado en el fichero indicado
		// por _fichero en almacenamiento persistente
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
