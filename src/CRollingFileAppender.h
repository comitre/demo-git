// Clase abstracta

#ifndef CROLLINGFILEAPPENDER_H_
#define CROLLINGFILEAPPENDER_H_

#include "CFileAppender.h"
#include <algorithm>

#define DEFAULT_FILE_PREFIX	"RFA_"

#define MIN_NUM_FILES		1
#define MAX_NUM_FILES		100

class CRollingFileAppender: public CFileAppender {

	public:

		CRollingFileAppender(std::string pathName,
				std::string filePrefix,
				unsigned int maxNumFiles,
				unsigned long long maxFileSize);

		virtual ~CRollingFileAppender(){};

		void setFilePrefix(std::string filePrefix) {this->filePrefix = filePrefix;};

		std::string getFilePrefix() const {return filePrefix;};

		void setMaxNumFiles(unsigned int maxNumFiles);
		void setMaxNumFiles(char* maxNumFiles){setMaxNumFiles(atoi(maxNumFiles));};
		unsigned int getMaxNumFiles() const {return maxNumFiles;};

		// Funcion virtual pura (que hace que la clase sea abstracta)
		virtual void write(std::string& msg, std::string& name, timeval* now, std::string& level) = 0;

	protected:

		void getFileNameVector(std::vector<std::string>&) const;
		void getLastFileName(std::string&) const;
		void cleanDirectory() const;
		void setFileName(std::string fileName) {this->fileName = fileName;};

	private:

		// Prefijo del fichero
		std::string filePrefix;

		// Numero maximo de ficheros que se mantienen en el directorio
		unsigned int maxNumFiles;
		static bool sortFileNames(std::string firstName, std::string secondName);
};

#endif /* CROLLINGFILEAPPENDER_H_ */
