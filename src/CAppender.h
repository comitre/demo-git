#ifndef CAPPENDER_H_
#define CAPPENDER_H_

#include <fstream>
#include <iomanip>
#include <vector>

class CAppender {

public:

	enum timeStampFormat {
		ISO8601 = 0x00,
		localFormat = 0x01,
		ISO8601_only_date = 0x02,
		ISO8601_only_time = 0x03,
		localFormat_only_date = 0x04,
		localFormat_only_time = 0x05
	};

	CAppender() {
		setName("");
		setWriteName(false);
		setWriteDate(false);
		setWriteLevel(false);
		setTimeStampFormat(localFormat);
	};

	virtual ~CAppender(){};
	virtual void write(std::string& msg, std::string& name, timeval* now, std::string& level) = 0;

	void setWriteName(bool writeName) {this->writeName = writeName;};
	void setWriteDate(bool writeDate) {this->writeDate = writeDate;};
	void setWriteLevel(bool writeLevel) {this->writeLevel = writeLevel;};

	void setWriteName(char* writeName) {setWriteName(!((std::string)"true").compare(writeName));};
	void setWriteDate(char* writeDate) {setWriteDate(!((std::string)"true").compare(writeDate));};
	void setWriteLevel(char* writeLevel) {setWriteLevel(!((std::string)"true").compare(writeLevel));};

	bool getWriteName() const {return writeName;};
	bool getWriteDate() const {return writeDate;};
	bool getWriteLevel() const {return writeLevel;};

	void setTimeStampFormat(timeStampFormat timeStampFormat) {this->timeStampFormat = timeStampFormat;};
	void setTimeStampFormat(char* timeStampFormat);
	timeStampFormat getTimeStampFormat() const {return timeStampFormat;};
	void setName(std::string name) {this->name = name;};
	std::string getName() const {return name;};

protected:

	std::string getTimeStamp(timeval *now) const;
	std::string getLayoutPrefix(timeval *now, std::string& name, std::string& level) const;
	timeStampFormat timeStampFormat;
	bool writeName, writeDate, writeLevel;
	std::string name;
};

#endif /* CAPPENDER_H_ */
