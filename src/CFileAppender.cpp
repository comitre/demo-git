#include "CFileAppender.h"
#include <experimental/filesystem>

// RECIBE:
//
//	pathName - Path en el que se genera el fichero de log
//	maxFileSize - Especifica el tamaño maximo que se permitira al fichero
//
// REALIZA: La construccion de un objeto CFileAppender
//
// RETORNA: Vacio
CFileAppender::CFileAppender(std::string pathName,  unsigned long long maxFileSize) {

	setPathName(pathName);
	setMaxFileSize(maxFileSize);
	file = new std::ofstream();
        // Si el path no existe, se crea
        // create_directories(static_cast<std::experimental::filesystem::path>(pathName));
}

// Destructor
CFileAppender::~CFileAppender() {

	if(file->is_open())
		file->close();
	delete file;
}

// RECIBE:
//
//	maxFileSize - El tamaño maximo a establecer al fichero
//
// REALIZA: Asegura que el valor este dentro de los limites y lo asigna al miembro
//
// RETORNA: Vacio
void CFileAppender::setMaxFileSize(unsigned long long maxFileSize) {

	// Se comprueba que el tamaño maximo este dentro de los limites permitidos
	if(maxFileSize < MIN_FILE_SIZE)
		maxFileSize = MIN_FILE_SIZE;
	else if(maxFileSize > MAX_FILE_SIZE)
		maxFileSize = MAX_FILE_SIZE;

	this->maxFileSize = maxFileSize;
}

// RECIBE:
//
//	append - true si el fichero se debe abrir para concatenar
//		falso si se debe abrir para crear de nuevo el fichero
//
// REALIZA: La apertura del fichero asociado al appender
//
// RETORNA: true - si se ha podido abrir el fichero, false - en otro caso
void CFileAppender::openFile(bool append) {

        create_directories(static_cast<std::experimental::filesystem::path>(getPathName()));

        std::string fullPath = getPathName() + '/' + getFileName();

        if(append)
                file->open(fullPath.c_str(), std::ios::app);
        else
                file->open(fullPath.c_str());

        if(file->is_open())
                file->seekp(0, std::ios::end);
}

// RECIBE:
//
//	now - Puntero a estructura timeval que almacena el instante actual
//
// REALIZA: La escritura en el fichero de las cadenas de inicio
//
// RETORNA: Vacio
void CFileAppender::writeInitString(timeval* now) {

	if(initString.size() > 0) {

		(*file)<< std::endl;

		for(unsigned int i = 0 ; i < initString.size() ; i++) {

			if(getWriteDate())
				(*file) << getTimeStamp(now) << ' ';

			(*file) << initString.at(i) << std::endl;
		}

		(*file)<< std::endl;
	}
}
