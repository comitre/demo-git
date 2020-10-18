#ifndef CLOGGER_H_
#define CLOGGER_H_

#include "CAppender.h"
#include "CLoggerStream.h"
#include <sys/time.h>
#include <map>
#include <mutex>

#define DEFAULT_NAME	"log"

class CLogger {

	public:

		typedef std::map<std::string, CAppender*> mapOfAppenders;
		typedef std::map<std::string, CLogger*> mapOfLoggers;

		enum level {
			l_none = 0x00,
			l_fatal = 0x01,
			l_error = 0x02,
			l_warn = 0x03,
			l_info = 0x04,
			l_debug = 0x05,
			l_trace = 0x06,
			l_always = 0x07
		};

		CLogger(std::string name = DEFAULT_NAME, level level = l_info);

		virtual ~CLogger(){};

		void always(std::string msg);
		void fatal(std::string msg);
		void error(std::string msg);
		void warn(std::string msg);
		void info(std::string msg);
		void debug(std::string msg);
		void trace(std::string msg);

		// void always(char* msg) {always(static_cast<std::string>(msg));};
		// void fatal(char* msg) {fatal(static_cast<std::string>(msg));};
		// void error(char* msg) {error(static_cast<std::string>(msg));};
		// void warn(char* msg) {warn(static_cast<std::string>(msg));};
		// void info(char* msg) {info(static_cast<std::string>(msg));};
		// void debug(char* msg) {debug(static_cast<std::string>(msg));};
		// void trace(char* msg) {trace(static_cast<std::string>(msg));};

		void always(char const* msg) {always(static_cast<std::string>(msg));};
		void fatal(char const* msg) {fatal(static_cast<std::string>(msg));};
		void error(char const* msg) {error(static_cast<std::string>(msg));};
		void warn(char const* msg) {warn(static_cast<std::string>(msg));};
		void info(char const* msg) {info(static_cast<std::string>(msg));};
		void debug(char const* msg) {debug(static_cast<std::string>(msg));};
		void trace(char const* msg) {trace(static_cast<std::string>(msg));};

		bool isTraceEnabled() const {return level > l_debug;};
		bool isDebugEnabled() const {return level > l_info;};
		bool isInfoEnabled() const {return level > l_warn;};
		bool isWarnEnabled() const {return level > l_error;};
		bool isErrorEnabled() const {return level > l_fatal;};
		bool isFatalEnabled() const {return level > l_none;};

		void setLevel(level level) {this->level = level;};
		void setLevel(unsigned char level);
		void setLevel(std::string level);
		level getLevel() const {return level;};

		void setName(std::string name) {this->name = name;};

		std::string getName() const {return name;};

		void addAppender(CAppender* appender) {linkedAppenders.push_back(appender);};

		CAppender* getLinkedAppender(std::string appenderName);

		static std::string& getStrLevel(level nivel) {return strNivel[nivel];};
		const static std::string getVersion() {return version;};

		static bool configure(std::string fileName);
		static void clear();
		// Es muy importante que esta funcion no sea inline
		static std::string getParseError();
		static CLogger* getLogger(std::string name);

		// Declaracion de los CLoggerStream, que permitiran enviar cadenas formateadas a las funciones homonimas
		CLoggerStream Always, Fatal, Error, Warn, Info, Debug, Trace;

	private:

		const static std::string version;
		static std::string strNivel[8];

		void setMessage(std::string& level, std::string& msg);

		/* Conjuntos de objetos CLogger, CAppender que se generaran en memoria cuando el logging se configure
		 * a partir de un fichero XML de configuracion */

		static mapOfLoggers* mapLogger;
		static mapOfAppenders* mapAppender;
		static std::ostringstream strSAXError;

		// Nombre, nivel y appenders ligados al logger
		std::string name;
		level level;
                std::vector<CAppender*> linkedAppenders;

                std::mutex escribiendo;
};

#endif /* CLOGGER_H_ */
