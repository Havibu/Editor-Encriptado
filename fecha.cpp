#include "fecha.hpp"

Fecha::Fecha(int dia, int mes, int anio, string dia_s):
_dia(dia), _mes(mes), _anio(anio), _dia_semana(dia_s) {

	// En caso de que no se especifique ningún dato, la fecha será
	// inicializada como la fecha de hoy.
	// Ejemplo formato de fecha: Wed Jun 30 21:49:08 1993\n
	//			      0123456789
	
	if(dia == -1){
		auto hoy = std::chrono::system_clock::now();
		std::time_t hoy_t = std::chrono::system_clock::to_time_t(hoy);
		string hoy_str(std::ctime(&hoy_t));
		std::stringstream datos;
		
		// Extraemos el día de la semana
		// man ctime: "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", and "Sat"
		string dia_semana(hoy_str.substr(0, 3));
		
		if(dia_semana == "Mon") _dia_semana = "Lunes";
		else if(dia_semana == "Tue") _dia_semana = "Martes";
		else if(dia_semana == "Wed") _dia_semana = "Miércoles";
		else if(dia_semana == "Thu") _dia_semana = "Jueves";
		else if(dia_semana == "Fri") _dia_semana = "Viernes";
		else if(dia_semana == "Sat") _dia_semana = "Sábado";
		else _dia_semana = "Domingo";
		
		// Extraemos el mes
		// man ctime: "Jan", "Feb", "Mar", "Apr", "May", "Jun",
		// "Jul", "Aug",  "Sep",  "Oct", "Nov",  and  "Dec"
		string mes_str(hoy_str.substr(4, 3));
		
		if(mes_str == "Jan") _mes = 1;
		else if(mes_str == "Feb") _mes = 2;
		else if(mes_str == "Mar") _mes = 3;
		else if(mes_str == "Apr") _mes = 4;
		else if(mes_str == "May") _mes = 5;
		else if(mes_str == "Jun") _mes = 6;
		else if(mes_str == "Jul") _mes = 7;
		else if(mes_str == "Aug") _mes = 8;
		else if(mes_str == "Sep") _mes = 9;
		else if(mes_str == "Oct") _mes = 10;
		else if(mes_str == "Nov") _mes = 11;
		else mes_str = 12;
		
		//Extraemos el número de día
		size_t posicion_hora;
		string dia_str(hoy_str.substr(8, (posicion_hora = hoy_str.find(" ", 8)) - 8));
		posicion_hora++;
		datos << dia_str << " ";
		
		//Extraemos el año
		string anio_str = hoy_str.substr(hoy_str.find(" ", posicion_hora) + 1, 4);
		datos << anio_str;
		
		datos >> _dia;
		datos >> _anio;
	}
	// En caso de que se hayan introducido todos los parámetros a excepción del día de la
	// semana, se establecerá el día de la semana a partir del resto de datos de la fecha
	else if(anio != -1 && dia_s == "MISSING DATA") set_dia_semana();
};
