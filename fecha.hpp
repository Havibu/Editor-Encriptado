#ifndef __FECHA_HPP
#define __FECHA_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream> //stringstream

using std::string;

class Fecha {
	private:
		int _dia; //Número de día del mes
		int _mes; //Número de mes
		int _anio; //Número de año
		string _dia_semana; //Día de la semana (lunes, martes, miércoles...)
		
	public:
		Fecha(int dia = -1, int mes = -1, int anio = -1, string dia_s = "MISSING DATA");
			
		//Devuelve el día del mes
		inline int get_dia() const { return _dia;};
		
		//Devuelve el mes del año
		inline int get_mes() const { return _mes;};
		
		//Devuelve el año
		inline int get_anio() const { return _anio;};
		
		//Devuelve el día de la semana
		inline string get_dia_semana() const {return _dia_semana;};
		
		//Devuelve la fecha siguiendo el formato: <día semana>, dd/mm/aaaa
		inline string get_fecha () const {
			std::stringstream fecha_ss;
			string fecha("");
			
			fecha_ss << _dia_semana << ", ";
			
			if(_dia < 10) fecha_ss << "0";
			fecha_ss << _dia << "/";
			
			if(_mes <  10 ) fecha_ss << "0";
			fecha_ss << _mes << "/" << _anio;
			
			fecha = fecha_ss.str();
			return fecha;
		};
		
		//Establece el número de día
		inline void set_dia(const int &nuevo_dia) { _dia = nuevo_dia;};
		
		//Establece el número de mes
		inline void set_mes(const int &nuevo_mes) { _mes = nuevo_mes;};
		
		//Establece el número de año
		inline void set_anio(const int &nuevo_anio) { _anio = nuevo_anio;};
		
		//Establece el día de la semana
		inline void set_dia_semana(const string &dia_semana) { _dia_semana = dia_semana;};
		
		inline void set_dia_semana(){
			int dia = _dia, mes = _mes, anio = _anio;
			std::string dias[] = {"Miércoles", "Jueves", "Viernes", "Sábado",
						"Domingo", "Lunes", "Martes"};
			mes = (mes + 9) % 12;
			anio -= mes / 10;
			unsigned long res = 365 * anio + anio / 4 - anio /100 + anio / 400 +
					    (mes * 306 + 5) / 10 + (dia - 1);
			
			_dia_semana = dias[res % 7];
		};
		
		//Establece una fecha determinada
		inline void set_fecha(int dia, int mes, int anio) {
			_dia = dia;
			_mes = mes;
			_anio = anio;
			set_dia_semana();
		};
		
		inline void set_fecha(Fecha fecha){
			_dia = fecha.get_dia();
			_mes = fecha.get_mes();
			_anio = fecha.get_anio();
			_dia_semana = fecha.get_dia_semana();
		};
		
		//Sobrecarga del operador =
		Fecha & operator=(const Fecha &fecha){
			_dia = fecha.get_dia();
			_mes = fecha.get_mes();
			_anio = fecha.get_anio();
			_dia_semana = fecha.get_dia_semana();
			
			return *this;
		}
};

#endif
