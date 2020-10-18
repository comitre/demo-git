#ifndef CSIZEFILEAPPENDER_H_
#define CSIZEFILEAPPENDER_H_

#include "CFileAppender.h"

#define DEFAULT_FILE_NAME	"logoutput.log"

class CSizeFileAppender: public CFileAppender {

	public:

		CSizeFileAppender(std::string pathName = DEFAULT_PATH_NAME,
				std::string fileName = DEFAULT_FILE_NAME,
				unsigned long maxFileSize = MIN_FILE_SIZE);

		virtual ~CSizeFileAppender(void){};

		void write(std::string& msg, std::string& name, timeval* now, std::string& level);

		void setFileName(std::string fileName) {this->fileName = fileName;};

	private:
		void rewriteFile(timeval*);
};

#endif /* CSIZEFILEAPPENDER_H_ */
