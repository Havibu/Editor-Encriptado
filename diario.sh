#!/bin/bash

hash_clave=""
hash_nuclear=""
clave=""
home_diario=/home/javier/Escritorio/diario/implementacion/Editor-Encriptado
tmpfs=/mnt/diario_tmp
almacenamiento=$home_diario/testing.enc
creacion=""
ultima_modificacion=""
group=""

get_group() {
	grupos=($(groups $USER))
	group=${grupos[2]}
}

get_hashes() {
	hashes=($(./hashes testing.enc))
	hash_clave=${hashes[0]}
	hash_nuclear=${hashes[1]}
}

prompt_clave() {
	clave=$(python3 clave.py $hash_clave $hash_nuclear)
}

# Dado que esto requiere que el script se ejecute como administrador, lo suyo es sustituirlo
# por un ejecutable con el bit ese especial que se ejecute como administrador
crear_directorio() {

	if [[ ! -d $tmpfs ]]; then
		mkdir $tmpfs
	fi
	
	mount -t tmpfs -o size=100M tmpfs $tmpfs
	chown $USER:$group $tmpfs
	chmod 755 $tmpfs
}

crear_documento() {
	$home_diario/generar_content_xml $almacenamiento $tmpfs $clave
	#run /home/javier/Escritorio/diario/implementacion/Editor-Encriptado/testing.enc /mnt/diario_tmp "hola"
	cp -r ./odt_files/* /mnt/diario_tmp
	cd $tmpfs
	zip -0 -X diario.doc mimetype
	zip -r diario.doc * -x cabecera.xml mimetype diario.doc
	creacion=$(stat -c '%y' $tmpfs/diario.doc)
	chown $USER:$group *
	ls | grep -Ev "(.*doc|.*xml)" | xargs -I{} rm -rf {}
}

abrir_documento() {
	libreoffice $tmpfs/diario.doc
}

modificado() {
	ultima_modificacion=$(stat -c '%y' $tmpfs/diario.doc)
	
	if [[ $ultima_modificacion == $creacion ]]; then 
		echo "false"
	else
		echo "true"
	fi
}

extraer_xml() {
	unzip $tmpfs/diario.doc -d $tmpfs
	ls $tmpfs | grep -v content.xml | xargs -I{} rm -rf {}
}

get_group
get_hashes
prompt_clave

# Clave nuclear
if [[ $clave == "nuclear" ]]; then
	
	echo "CLAVE NUCLEAR!"	

# Clave errónea
elif [[ $clave == "ERROR" ]]; then

	echo "ERROR::DIARIO - CONTRASEÑA INCORRECTA"

# Clave correcta
else
	echo "CLAVE CORRECTA!"
	crear_directorio
	crear_documento
	abrir_documento
	
	if [[ $(modificado) == true ]]; then
		rm -rf *.xml
		extraer_xml
		$home_diario/actualizar_diario $tmpfs $almacenamiento $clave
		#run /mnt/diario_tmp /home/javier/Escritorio/diario/implementacion/Editor-Encriptado/testing.enc "hola"
	fi
	
	#rm -rf $tmpfs/*
fi



