#ifndef CROLLINGTIMEFILEAPPENDER_H_
#define CROLLINGTIMEFILEAPPENDER_H_

#include "CRollingFileAppender.h"

class CRollingTimeFileAppender: public CRollingFileAppender {

	public:

		enum rollingPeriod {
			year = 0x00,
			month = 0x01,
			day = 0x02,
			hour = 0x03,
			minute = 0x04
		};

		CRollingTimeFileAppender(std::string pathName = DEFAULT_PATH_NAME,
				std::string filePrefix = DEFAULT_FILE_PREFIX,
				unsigned int maxNumFiles = MIN_NUM_FILES,
				unsigned long long maxFileSize = MAX_FILE_SIZE,
				rollingPeriod rollingPeriod = day);

		~CRollingTimeFileAppender(void){};

		void setRollingPeriod(rollingPeriod rollingPeriod) {this->rollingPeriod = rollingPeriod;};
		void setRollingPeriod(std::string rollingPeriod);

		rollingPeriod getRollingPeriod() const {return rollingPeriod;};

		void write(std::string& msg, std::string& name, timeval* now, std::string& level);

	private:

		// Periodo para generacion de nuevo fichero
		rollingPeriod rollingPeriod;

		std::string getNewFileName(timeval*) const;
};

#endif /* CROLLINGTIMEFILEAPPENDER_H_ */
