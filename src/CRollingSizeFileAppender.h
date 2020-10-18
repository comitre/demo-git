#ifndef CROLLINGSIZEFILEAPPENDER_H_
#define CROLLINGSIZEFILEAPPENDER_H_

#include "CRollingFileAppender.h"

class CRollingSizeFileAppender: public CRollingFileAppender {

	public:

		CRollingSizeFileAppender(std::string pathName = DEFAULT_PATH_NAME,
				std::string filePrefix = DEFAULT_FILE_PREFIX,
				unsigned int maxNumFiles = MIN_NUM_FILES,
				unsigned long long maxFileSize = MAX_FILE_SIZE) : CRollingFileAppender(pathName, 
					filePrefix, maxNumFiles, maxFileSize) {index = 0;};

		~CRollingSizeFileAppender(void){};

		void write(std::string& msg, std::string& name, timeval* now, std::string& level);

	private:

		std::string getNewFileName();
		std::string getCurrentFileName();

		unsigned int index;
};

#endif /* CROLLINGSIZEFILEAPPENDER_H_ */
