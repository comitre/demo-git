#include "CSizeFileAppender.h"

// RECIBE:
//
//	pathName - Nombre del path en el que se generan los ficheros
//	fileName - Nombre del fichero en el que se escribe el log
//	maxFileSize - Especifica el tamaño maximo que se permitira al fichero
//
// REALIZA: La construccion de un objeto CFileAppender
//
// RETORNA: Vacio
CSizeFileAppender::CSizeFileAppender(std::string pathName, std::string fileName, 
		unsigned long maxFileSize) : CFileAppender(pathName, maxFileSize) {
	setFileName(fileName);
}

// RECIBE:
//
//	now - Puntero a estructura SYSTEMTIME que almacena el instante actual
//
// REALIZA: La reescritura del fichero (tipicamente se hace cuando alcanza el tamaño maximo)
//
// RETORNA: Vacio
void CSizeFileAppender::rewriteFile(timeval* now) {

	file->close();
	if(!file->is_open()) {
		openFile(false);
		if(file->is_open())
			writeInitString(now);
	}
}

// RECIBE:
//
//	msg - Cadena a escribir en el appender
//	name - Nombre del logger
//	now - El instante del suceso a registrar
//	level - cadena que representa el nivel de logging
//
// REALIZA: Construye la cadena que se escribira en fichero en funcion del layout
//
// RETORNA: Vacio
void CSizeFileAppender::write(std::string& msg, std::string& name, timeval* now, std::string& level) {

	// Segun el layout se construye la cadena a escribir
	std::string message = getLayoutPrefix(now, name, level) + msg;

	seccionCritica.lock();

	if(!file->is_open()) {
		openFile();
		writeInitString(now);
	}

	if(file->is_open()) {

		/* Se escribe en fichero siempre y cuando no se haya alcanzado el tamaño maximo
		 * IMPORTANTE el cast a long long para que el calculo sea correcto */
		if((long long)file->tellp() > (long long)getMaxFileSize() - message.length())
			rewriteFile(now);

		(*file)<< message << std::endl;
		file->flush();
	}

	seccionCritica.unlock();
}
