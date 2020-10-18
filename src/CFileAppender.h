// Clase abstracta

#ifndef CFILEAPPENDER_H_
#define CFILEAPPENDER_H_

#include <mutex>
#include "CAppender.h"

#define DEFAULT_PATH_NAME	"."
#define MIN_FILE_SIZE		65536		//Tamaño minimo 64 Kb
#define MAX_FILE_SIZE		10737418240	//Tamaño maximo 10 Gb

class CFileAppender : public CAppender {

	public:

		CFileAppender(std::string pathName, unsigned long long maxFileSize);

		virtual ~CFileAppender();

		void setPathName(std::string pathName) {this->pathName = pathName;};
		std::string getPathName() const {return pathName;};

		std::string getFileName() const {return fileName;};

		void setMaxFileSize(unsigned long long maxFileSize);
		void setMaxFileSize(char* maxFileSize){setMaxFileSize(atol(maxFileSize));};

		unsigned long long getMaxFileSize() const {return maxFileSize;};

		void addInitString(std::string initStr) {initString.push_back(initStr);};

		void modifyInitString(unsigned int index, std::string initStr) {
			if(index < initString.size()) initString.at(index) = initStr;
		};

		// Funcion virtual pura (que hace que la clase sea abstracta)
		void write(std::string& msg, std::string& name, timeval* now, std::string& level) = 0;

	protected:

                std::ofstream* file;
		std::string fileName;
                std::mutex seccionCritica;

		void openFile(bool append = true);
		void writeInitString(timeval*);

	private:

		std::string pathName;
		unsigned long long maxFileSize;
                std::vector<std::string> initString;
};

#endif /* CFILEAPPENDER_H_ */
