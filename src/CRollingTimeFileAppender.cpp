#include "CRollingTimeFileAppender.h"

// RECIBE:
// 
//	pathName - Ruta del fichero en el que se escribe el log
//	filePrefix - Prefijo del fichero
//	maxNumFiles - Especifica el numero maximo de ficheros que se mantiene en el directorio
//	maxFileSize - Especifica el tamaño maximo que se permite al fichero
//	rollingPeriod - Periodo de tiempo tras el cual se genera un nuevo fichero
//
// REALIZA: La construccion de un objeto CRollingTimeFileAppender
//
// RETORNA: Vacio
CRollingTimeFileAppender::CRollingTimeFileAppender(std::string pathName,
		std::string filePrefix,
		unsigned int maxNumFiles,
		unsigned long long maxFileSize,
		enum rollingPeriod rollingPeriod) : CRollingFileAppender(pathName, filePrefix, maxNumFiles, maxFileSize) {

	setRollingPeriod(rollingPeriod);
}

// RECIBE:
//
//	rollingPeriod - Cadena de texto que representa el periodo de tiempo tras el cual se genera un nuevo fichero
//
// REALIZA: Establece el RollingPeriod segun la cadena recibida como parametro
//
// RETORNA: Vacio
void CRollingTimeFileAppender::setRollingPeriod(std::string rollingPeriod) {

	if(!rollingPeriod.compare("year"))
		setRollingPeriod(year);
	else if(!rollingPeriod.compare("month"))
		setRollingPeriod(month);
	else if(!rollingPeriod.compare("hour"))
		setRollingPeriod(hour);
	else if(!rollingPeriod.compare("minute"))
		setRollingPeriod(minute);
	else // Si ninguno de los anteriores, se consideran ficheros diarios
		setRollingPeriod(day);
}

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
void CRollingTimeFileAppender::write(std::string& msg, std::string& name, timeval* now, std::string& level) {

	std::string currentFileName, message;
	bool newFile = false;

	seccionCritica.lock();

	// Segun el layout se construye la cadena a escribir
	message = getLayoutPrefix(now, name, level) + msg;

	// Se obtiene el nombre de fichero que deberia estar activo en este momento
	currentFileName = getNewFileName(now);

	if(currentFileName != getFileName()) {

		// Creacion de un nuevo fichero.

		setFileName(currentFileName);

		// Se cierra el fichero viejo
		if(file->is_open())
			file->close();

		newFile = true;
	}

	// Se comprueba si el fichero esta abierto (podria estar cerrado por desconexion intempestiva de dispositivo
	if(!file->is_open())
		openFile();

	// Despues de cada "openFile" se comprueba si el fichero se pudo abrir
	if(file->is_open()) {

		if(newFile) {

			cleanDirectory();
			writeInitString(now);

		}

		/* Se escribe en fichero siempre y cuando no se haya alcanzado el tamaño maximo
		 * IMPORTANTE el cast a long long para que el calculo sea correcto */
		if((long long)file->tellp() < (long long)getMaxFileSize() - message.length()) {

			(*file)<< message << std::endl;
			file->flush();
		}
	}

	seccionCritica.unlock();
}

// RECIBE.: now - Un puntero a estructura SYSTEMTIME para obtener la fecha y la hora del instante actual
// 
// REALIZA: Construye la cadena que representa el nombre del fichero en funcion de la fecha y la hora
//
// RETORNA: dicha cadena
std::string CRollingTimeFileAppender::getNewFileName(timeval* now) const {

	std::ostringstream strNewFile;

	struct tm* date = localtime(&now->tv_sec);

	strNewFile << std::setfill('0')
	       	<< getFilePrefix()
	       	<< std::setw(4)
	       	<< date->tm_year + 1900;

	switch(rollingPeriod) {

		case month: // Mes

			strNewFile << '-' << std::setw(2)
			       	<< date->tm_mon + 1;
			break;

		case day: // Dia

			strNewFile << '-' << std::setw(2)
			       	<< date->tm_mon + 1
			       	<< '-' << std::setw(2)
			       	<< date->tm_mday;
			break;

		case hour: // Hora

			strNewFile << '-'
			       	<< std::setw(2)
			       	<< date->tm_mon + 1
			       	<< '-'
			       	<< std::setw(2)
			       	<< date->tm_mday 
				<< '-'
			       	<< std::setw(2)
			       	<< date->tm_hour;
			break;

		case minute: // Minuto

			strNewFile << '-'
			       	<< std::setw(2)
			       	<< date->tm_mon + 1
			       	<< '-' << std::setw(2)
			       	<< date->tm_mday 
				<< '-'
			       	<< std::setw(2)
			       	<< date->tm_hour
			       	<< '-'
			       	<< std::setw(2)
			       	<< date->tm_min;
			break;

		default: // Año (nada que hacer)

			break;
	}

	strNewFile << ".log";

	return strNewFile.str();
}
