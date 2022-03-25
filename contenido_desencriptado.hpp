#ifndef __CONTENIDO_DESENCRIPTADO_HPP
#define __CONTENIDO_DESENCRIPTADO_HPP

#include "fecha.hpp"
#include "entrada.hpp"
#include "aes.hpp"
#include <list>
#include <string>
#include <cstdint> //uint8_t

using std::list;
using std::string;

class ContenidoDesencriptado {

	private:
		list<Entrada> _entradas; // Entradas extraídas del content.xml
		unsigned char * _clave; // Clave de desencriptado
		uint8_t * _contenido_desencriptado; //Contenido desencriptado
		size_t _longitud;
		
	public:
		// Constructor de la clase
		inline ContenidoDesencriptado(){
			_clave = nullptr;
		}
	
		// Devuelve la lista de entradas
		inline list<Entrada> get_entradas() const {return _entradas;};
		
		// Devuelve la clave de encriptado (TODO(W): testing?)
		inline unsigned char * get_pass() const {return _clave;};
		
		// Devuelve el contenido desencriptado (testing?)
		inline uint8_t * get_contenido() const {return _contenido_desencriptado;};
		
		// Devuelve la última entrada de la lista de entradas
		inline Entrada get_ultima_entrada() const {return _entradas.back();};
		
		// Establece la clave de desencriptado
		void set_clave(const string);
		
		void set_clave (const unsigned char *);
		
		// Desencripta el contenido pasador por parámetro y lo almacena en _contenido
		bool desencriptar(const uint8_t *, const size_t);
		
		// Extrae las entradas del contenido desencriptado
		bool extraer_entradas();
		
		// Destructor de la clase
		inline ~ContenidoDesencriptado() {
			if(_clave != nullptr)
				delete [] _clave;
		};
};

#endif
