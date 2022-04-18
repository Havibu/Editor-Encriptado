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

void ContenidoDesencriptado::extraer_entradas() {
		
	size_t pos = 0 ;
	list<string> entradas_str;
	list<string>::iterator entrada_i;
	string entrada_str, dia, mes, anio, contenido;
	
	// Obtenemos las entradas en forma de cadena de texto
	for(size_t i = 0; i < _longitud; i++) {
		
		entrada_str.push_back(_contenido_desencriptado[i]);
			
		if(_contenido_desencriptado[i] == (unsigned char)'\222') {
			entradas_str.push_back(entrada_str);
			entrada_str.clear();
		}
	}
	
	// Convertimos las entradas de texto a objeto Entrada y las almacenamos en
	// la lista de entradas
	for(entrada_i = entradas_str.begin(); entrada_i != entradas_str.end(); entrada_i++, dia.clear(), mes.clear(), anio.clear(), contenido.clear(), pos = 0){
		
		// Extraemos el día
		for(; (*entrada_i)[pos] != '\220'; pos++)
			dia.push_back((*entrada_i)[pos]);
		
		// Extraemos el mes
		for(pos++; (*entrada_i)[pos] != '\220'; pos++)
			mes.push_back((*entrada_i)[pos]);
			
		// Extraemos el año
		for(pos++; (*entrada_i)[pos] != '\221'; pos++)
			anio.push_back((*entrada_i)[pos]);
		
		// Creamos la fecha de la entrada
		Fecha fecha(dia, mes, anio);
		pos++;
		
		// Extraemos el contenido
		for(int i = 0; (*entrada_i)[pos] != '\222'; i++, pos++)
			contenido.push_back((*entrada_i)[pos]);
		
		// Creamos la entrada
		Entrada entrada(fecha, contenido);
		
		// La añadimos a la lista
		_entradas.push_back(entrada);
	}
}

bool ContenidoDesencriptado::generar_xml(const string directorio) {

	string fichero = directorio + "/content.xml";
	
	// Elementos de un fichero content.xml
	string cabecera_fecha = "<text:p text:style-name=\"fecha\">";
	string cabecera_contenido = "<text:p text:style-name=\"contenido\">";
	string salto = "<text:p text:style-name=\"P1\"/>";
	string foot_texto = "</text:p>";
	string foot_content = "</office:text></office:body></office:document-content>";
	
	list<Entrada>::const_iterator entrada;
	std::stringstream contenido;
	char *cabecera_str;
	string contenido_str;
	bool cerrado = false;
	Fecha hoy;
	
	try {
		// Creamos el content.xml
		std::fstream content(fichero, std::ios_base::out);
		std::streambuf * content_xml = content.rdbuf();
		
		// Extraemos la cabecera para el content.xml
		std::fstream cabecera_file("./odt_files/cabecera.xml", std::ios_base::in);
		std::streambuf * cabecera = cabecera_file.rdbuf();
		
		// Sacamos el tamaño de la cabecera
		std::streamsize tamanio;
		tamanio = cabecera->pubseekoff(0, cabecera_file.end);
		cabecera_str = new char[tamanio];
		
		// Volvemos al principio del fichero
		cabecera->pubseekoff(0, cabecera_file.beg);
		
		// Extraemos toda la cabecera
		cabecera->sgetn(cabecera_str, tamanio);
		
		// Nos movemos a la última entrada
		entrada = _entradas.end();
		entrada--;
			
		// Comprobamos que el contenido de la entrada no esté vacío
		if(entrada->get_contenido().empty()){
			
			// Si la última entrada está vacía y la fecha es anterior a la de hoy,
			// se trata de una entrada que se creó en un principio pero que nunca
			// se llegó a introducir contenido, de forma que la eliminamos para
			// no llenar el documento con entradas vacías
			if(entrada->get_fecha().get_fecha() != hoy.get_fecha())
			    _entradas.pop_back();
		}
		
		// Comprobamos que la última entrada corresponde al día de hoy,
		// y si no es así, creamos una
		if(entrada->get_fecha().get_fecha() != hoy.get_fecha()){
			Entrada entrada_hoy(hoy);
			_entradas.push_back(entrada_hoy);
		}
		
		// Convertimos las entradas en lista a formato XML de ODT:
		
		// Escribimos la cabecera del content.xml
		contenido << cabecera_str;
		
		for(entrada = _entradas.begin(); entrada != _entradas.end(); entrada++){
		    string contenido_entrada = entrada->get_contenido();
		
		    if(entrada != _entradas.begin()) contenido << salto;
			
		    contenido << cabecera_fecha << entrada->get_fecha().get_fecha() 
			      << foot_texto << salto << cabecera_contenido;
			      
		    cerrado = false;
		    
		    for(size_t i = 0; i < contenido_entrada.length(); i++){
			if(contenido_entrada[i] == '\n'){
			    contenido << foot_texto;
			    cerrado = true;
					
			    while(i+1 < contenido_entrada.length() && contenido_entrada[i+1] == '\n'){
			    	i++;
				contenido << salto;
			    }
			    
			    if(i+1 != contenido_entrada.length()) {
				contenido << cabecera_contenido;
				cerrado = false;
			    }
			}
			else
			    contenido << contenido_entrada[i];
		    }
		    if(!cerrado){
		    	contenido << foot_texto;
		    	cerrado = true;
		    }
		}
		contenido << foot_content;
		
		// Volvamos el contenido al content.xml
		contenido_str = contenido.str();
		content_xml->sputn((const char *)contenido_str.c_str(), contenido_str.length());
		
		// Cerramos ficheros
		content.close();
		cabecera_file.close();
	}
	catch(...){
		return false;
	}

	return true;
}

// Extrae el tag y deja la variable pos en la posición donde o bien empezaría el contenido
// de ese tag, o bien si se trata de un tag vacío en la posición donde comienza el
// siguiente tag
string extraer_tag(const char *xml, size_t &pos) {
	string tag;

	for(; xml[pos] != '>'; pos++)
		tag.push_back(xml[pos]);
	
	tag.push_back('>');
	pos++;
		
	return tag;
}

bool ContenidoDesencriptado::actualizar_entradas(const string dir) {

	string fichero = dir + "/content.xml", tag;
	string cabecera_fecha = "<text:p text:style-name=\"fecha\">";
	string cabecera_contenido = "<text:p text:style-name=\"contenido\">";
	size_t pos = 0, tamanio;
	char *contenido_str;
	bool contenido = false;
	string fecha, contenido_entrada;
	Fecha fecha_entrada;
	Entrada entrada;
	
	std::fstream content_file(fichero, std::ios_base::in);
	std::filebuf * content_buf = content_file.rdbuf();
	
	// Obtenemos el tamaño del content.xml
	tamanio = (size_t) content_buf->pubseekoff(0, content_file.end);
	
	// Volvemos al principio
	content_buf->pubseekoff(0, content_file.beg);
	
	// Extraemos el contenido de content.xml y cerramos el fichero
	contenido_str = new char[tamanio];
	content_buf->sgetn(contenido_str, tamanio);
	content_file.close();
	
	// Borramos la lista de entradas
	_entradas.clear();
	
	// Extraemos entradas XML y su contenido hasta que alcancemos el final
	while( (tag = extraer_tag(contenido_str, pos)) != "</office:body>" ){
	
		if(tag == cabecera_fecha){			
			if(contenido == true){
				// Añadimos a la lista la entrada extraída
				entrada.set_fecha(fecha_entrada);
				contenido_entrada.pop_back(); //(?)
				entrada.set_contenido(contenido_entrada);
				_entradas.push_back(entrada);
				
				fecha.clear();
				contenido_entrada.clear();
			}
			
			// Indicamos que estamos extrayendo una fecha y no el contenido
			// de una entrada
			contenido = false;
			
			// Extraemos la fecha de la entrada
			for(; contenido_str[pos] != '<'; pos++)
				fecha.push_back(contenido_str[pos]);
				
			// La convertimos a objeto Fecha
			fecha_entrada.set_fecha(fecha);
			
			// Buscamos el contenido de la entrada
			do{tag = extraer_tag(contenido_str, pos);}
			while( (tag == cabecera_fecha || tag.find("/") != string::npos) && tag != "</office:text>" );
			
			contenido = true;
			
			// Extraemos el contenido de la entrada
			if(tag != "</office:text>")
			    for(; contenido_str[pos] != '<'; pos++)
				contenido_entrada.push_back(contenido_str[pos]);
		}
		
		// Si estamos extrayendo contenido y nos encontramos un salto
		else if(contenido == true) {
		
		    if( (tag.find("/") != string::npos && tag[tag.length() - 2] == '/') || tag == "</text:p>")
			    contenido_entrada.push_back('\n');
			    
		    for(; contenido_str[pos] != '<'; pos++)
			contenido_entrada.push_back(contenido_str[pos]);
		}
	}
	
	// Si nos falta la última entrada por añadir, la añadimos
	if(!fecha.empty()){
		entrada.set_fecha(fecha_entrada);
		entrada.set_contenido(contenido_entrada);
		_entradas.push_back(entrada);
	}

	return true;
}








