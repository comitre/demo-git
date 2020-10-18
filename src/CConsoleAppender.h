#ifndef CCONSOLEAPPENDER_H_
#define CCONSOLEAPPENDER_H_

#include "CAppender.h"
#include <iostream>
#include <mutex>

class CConsoleAppender : public CAppender {

	public:

		CConsoleAppender() {};
		virtual ~CConsoleAppender(){};
		void write(std::string& msg, std::string& name, timeval* now, std::string& level);

	private:

                std::mutex outputMutex;
};

#endif /* CCONSOLEAPPENDER_H_ */
