#include "CRollingSizeFileAppender.h"

// RECIBE:
//
//	msg - Cadena a escribir en el fichero
//	name - Nombre del logger
//	now - El instante del suceso a registrar
//	level - Cadena que representa el nivel de logging
//
// REALIZA: Construye la cadena que se escribira en fichero
//
// RETORNA: Vacio
void CRollingSizeFileAppender::write(std::string& msg, std::string& name, timeval* now, std::string& level) {

	std::string message;
	bool newFile = false;

	seccionCritica.lock();

	// Segun el layout se construye la cadena a escribir
	message = getLayoutPrefix(now, name, level) + msg;

	// Se comprueba si el fichero esta abierto. Podria estar cerrado si se trata de la primera escritura)
	if(!file->is_open()) {

		// Si no hay establecido un nombre de fichero se establece el nombre inicial (primera apertura)
		if(!getFileName().compare("")) {
			setFileName(getCurrentFileName());
			newFile = true;
		}

		openFile();
	}

	// Despues de cada "openFile" se comprueba si el fichero se pudo abrir
	if(file->is_open()) {

		/* Se escribe en fichero siempre y cuando no se haya alcanzado el tamaño maximo
		 * IMPORTANTE el cast a long long para que el calculo sea correcto */
		if((long long)file->tellp() > (long long)getMaxFileSize() - message.length()) {

			// Se ha alcanzado el tamaño maximo, por tanto se crea un nuevo fichero.

			setFileName(getNewFileName());

			newFile = true;

			// Se cierra el fichero viejo
			file->close();

			// Si se ha podido cerrar, se crea de nuevo con el nuevo nombre
			if(!file->is_open())
				openFile();
		}

		// Despues de cada "openFile" se comprueba si el fichero se pudo abrir
		if(file->is_open()) {

			if(newFile) {

				/* Si se esta procesando un nuevo fichero se limpia el directorio y
				 * se escriben las cadenas de inicio*/
				cleanDirectory();
				writeInitString(now);
			}

			// Finalmente se escribe el texto en el fichero
			(*file)<< message << std::endl;
			file->flush();
		}
	}

	seccionCritica.unlock();
}

// RECIBE:
//
//	Vacio
//
// REALIZA: Construye la cadena que representa el nombre del fichero en funcion del indice numerico en curso
//
// RETORNA: dicha cadena
std::string CRollingSizeFileAppender::getNewFileName() {

	std::ostringstream strNewFile;

	index++;
	strNewFile << getFilePrefix() << std::setfill('0') << std::setw(10) << index << ".log";
	return strNewFile.str();
}

// RECIBE:
//
//	Vacio
//
// REALIZA: Construye la cadena que representa el nombre del ultimo fichero en el directorio, y si este
//		esta vacio inicializa el indice y retorna el primer nombre de fichero
//
// RETORNA: dicha cadena
std::string CRollingSizeFileAppender::getCurrentFileName() {

	std::string fileName;

	getLastFileName(fileName);

	if(!fileName.compare("")) {
		index = 0;
		return getNewFileName();
	}

	index = atoi(fileName.substr(getFilePrefix().length(), 10).c_str());
	return fileName;
}

