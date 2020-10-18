#include "CRollingFileAppender.h"
#include <dirent.h>
#include <cerrno>
#include <iostream>

// RECIBE:
//
//	pathName - Ruta del fichero en el que se escribe el log
//	filePrefix - Prefijo del fichero
//	maxNumFiles - Especifica el numero maximo de ficheros que se mantiene en el directorio
//	maxFileSize - Especifica el tamaño maximo que se permite al fichero
//
// REALIZA: La construccion de un objeto CFileRollingAppender
//
// RETORNA: Vacio
CRollingFileAppender::CRollingFileAppender(std::string pathName,
		std::string filePrefix,
		unsigned int maxNumFiles,
		unsigned long long maxFileSize) : CFileAppender(pathName, maxFileSize) {

	setFileName("");
	setFilePrefix(filePrefix);
	setMaxNumFiles(maxNumFiles);
}

// RECIBE:
//
//	maxNumFiles - El numero maximo de ficheros a mantener en el directorio
//
// REALIZA: Asegura que el valor este dentro de los limites y lo asigna al miembro
//
// RETORNA: Vacio
void CRollingFileAppender::setMaxNumFiles(unsigned int maxNumFiles) {

	// Se comprueba que el tamaño maximo este dentro de los limites permitidos
	if(maxNumFiles < MIN_NUM_FILES)
		maxNumFiles = MIN_NUM_FILES;
	else if(maxNumFiles > MAX_NUM_FILES)
		maxNumFiles = MAX_NUM_FILES;

	this->maxNumFiles = maxNumFiles;
}

// RECIBE:
//
//	firstName - primera cadena de texto a comparar
//	secondName - segunda cadena de texto a comparar
//
// REALIZA: Compara alfabeticamente dos cadenas de texto
//
// RETORNA: 'true' si la primera cadena es menor que la segunda, 'false' en otro caso
bool CRollingFileAppender::sortFileNames(std::string firstName, std::string secondName) {
	return firstName < secondName;
}

// RECIBE:
//
//	fileNameVector - Referencia a un vector en el que se obtendran los nombres de fichero
//
// REALIZA: Escanea el directorio y rellena el vector recibido con los nombres de aquellos ficheros cuyo
//		prefijo coincida con el de los ficheros de log en cuestion
//
// RETORNA: vacio
void CRollingFileAppender::getFileNameVector(std::vector<std::string>& fileNameVector) const {

	int numEntries;
	struct dirent **namelist, **list;
	std::string currentFile;

	fileNameVector.clear();

	if((numEntries = scandir(getPathName().c_str(), &namelist, NULL, alphasort)) < 0) {

		std::cerr << "CFileRollingHourAppender - Error: (scandir). Directorio: " << getPathName() << std::endl;

	} else {

		list = namelist;
		for(int i = 0 ; i < numEntries ; i++) {

			currentFile = (*list)->d_name;

			if(currentFile.substr(0, getFilePrefix().length()) == getFilePrefix())
				fileNameVector.push_back(currentFile);

			free(*list);
			list++;
		}

		free(namelist);
	}
}

// RECIBE:
//
//	Vacio
// 
// REALIZA: Limpia el directorio, manteniendo solamente el numero maximo de ficheros especificado
//
// RETORNA: Vacio
void CRollingFileAppender::cleanDirectory() const {
 
        std::vector<std::string> fileNameVector;

	getFileNameVector(fileNameVector);

	if(fileNameVector.size() > getMaxNumFiles()) {

		// Ordenar el vector (EN LINUX CREO QUE NO HACE FALTA YA QUE VIENE ORDENADO...
		//sort(fileNameVector.begin(), fileNameVector.end(), sortFileNames);

		unsigned int numFilesToRemove = fileNameVector.size() - getMaxNumFiles();

		std::string currentFile;
		for(unsigned int i = 0 ; i < numFilesToRemove ; i++) {

			currentFile = getPathName() + fileNameVector[i];
			if(remove(currentFile.c_str()))
				std::cerr << "CFileRollingHourAppender - Error: "
				       << errno
				       << " (remove). Fichero: "
				       << currentFile << std::endl;
		}
	}
}

// RECIBE:
//
//	fileName - Una referencia a una cadena de texto donde se devolvera el nombre del fichero
//
// REALIZA: Busca el nombre del fichero mas nuevo en el directorio del "RollingFileAppender"
//
// RETORNA: Vacio
void CRollingFileAppender::getLastFileName(std::string& fileName) const {

        std::vector<std::string> fileNameVector;

	fileName.clear();

	getFileNameVector(fileNameVector);

	if(fileNameVector.size() > 0) {

		// Ordenar el vector (EN LINUX CREO QUE NO HACE FALTA YA QUE VIENE ORDENADO...
		//sort(fileNameVector.begin(), fileNameVector.end(), sortFileNames);
		fileName = fileNameVector.back();
	}
}
