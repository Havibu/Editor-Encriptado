#ifndef __ENTRADA_HPP
#define __ENTRADA_HPP

#include <string>
#include "fecha.hpp"

using std::string;

class Entrada {
	private:
		Fecha _fecha; //Fecha asociada a la entrada
		string _contenido; //Contenido desencriptado de la entrada
		
	public:
		//Devuelve la fecha asociada a la entrada
		inline Fecha get_fecha() const {return _fecha;};
		
		//Devuelve el contenido desencriptado de la entrada
		inline string get_contenido() const {return _contenido;};
		
		//Establece la fecha asociada a la entada
		inline void set_fecha(const Fecha &nueva_fecha) {_fecha = nueva_fecha;};
		
		//Establece el contenido desencriptado de la entrada
		inline void set_contenido(const string &nuevo_contenido) {_contenido = nuevo_contenido;};
};

#endif
