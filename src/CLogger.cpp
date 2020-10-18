#include "CLogger.h"
#include "CSAX2Handler.h"

// Inicializacion de las variables estaticas
const std::string CLogger::version = "Nordés Logger 2.1.0 - dom nov 10 10:42:57 CET 2019";

std::string CLogger::strNivel[8] = {"none", "fatal", "error", "warn", "info", "debug", "trace", "always"};

CLogger::mapOfLoggers* CLogger::mapLogger;
CLogger::mapOfAppenders* CLogger::mapAppender;
std::ostringstream CLogger::strSAXError;

// RECIBE:
//
//	name - El nombre del logger
//	level - El nivel de trabajo
//
// REALIZA: Construye un objeto de tipo CLogger
//
// RETORNA: vacio
CLogger::CLogger(std::string name, enum level level) : name(name), level(level) {

	// Inicializacion de los streams
	Always.setLogger(this);
	Fatal.setLogger(this);
	Error.setLogger(this);
	Warn.setLogger(this);
	Info.setLogger(this);
	Debug.setLogger(this);
	Trace.setLogger(this);

	// Asignacion de funcion de volcado a los streams 
	Always.setDumpFunction(&CLogger::always);
	Fatal.setDumpFunction(&CLogger::fatal);
	Error.setDumpFunction(&CLogger::error);
	Warn.setDumpFunction(&CLogger::warn);
	Info.setDumpFunction(&CLogger::info);
	Debug.setDumpFunction(&CLogger::debug);
	Trace.setDumpFunction(&CLogger::trace);
};

// RECIBE:
//
//	level - Una cadena que representa el nivel de Logger
//
// REALIZA: Establece el nivel de logger
//
// RETORNA: vacio
void CLogger::setLevel(std::string level) {

	if(!level.compare("none"))
		setLevel(l_none);
	else if(!level.compare("fatal"))
		setLevel(l_fatal);
	else if(!level.compare("error"))
		setLevel(l_error);
	else if(!level.compare("warn"))
		setLevel(l_warn);
	else if(!level.compare("debug"))
		setLevel(l_debug);
	else if(!level.compare("trace")){
		setLevel(l_trace);
	}
	else // Por defecto se sobreentiende info
		setLevel(l_info);
}

// RECIBE:
// 
//	level - Un unsigned char que representa el nivel de Logger
//
// REALIZA: Establece el nivel de logger
//
// RETORNA: vacio
void CLogger::setLevel(unsigned char level) {

	switch(level) {

		case l_none:

			setLevel(l_none);
			break;

		case l_fatal:

			setLevel(l_fatal);
			break;

		case l_error:

			setLevel(l_error);
			break;

		case l_warn:

			setLevel(l_warn);
			break;

		case l_debug:

			setLevel(l_debug);
			break;

		case l_trace:

			setLevel(l_trace);
			break;

		default: // Ninguno de los anteriores, se sobreentiende info
			setLevel(l_info);
	}
}

// RECIBE:
//
//	appenderName - El nombre de un appender que se supone enlazado al logger
//
// REALIZA: busca el appender cuyo nombre recibe como parametro
//
// RETORNA: Si el appender existe un puntero al mismo, NULL en otro caso
CAppender* CLogger::getLinkedAppender(std::string appenderName) {

	for(unsigned char i = 0 ; i < linkedAppenders.size() ; i++)
		if(!linkedAppenders[i]->getName().compare(appenderName))
			return linkedAppenders[i];

	return NULL;
};

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'always' (es decir se escribe siempre)
//
// RETORNA: vacio
void CLogger::always(std::string msg) {
	setMessage(getStrLevel(l_always), msg);
}

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'fatal'
//
// RETORNA: vacio
void CLogger::fatal(std::string msg) {

	if(level >= l_fatal)
		setMessage(getStrLevel(l_fatal), msg);
}

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'error'
//
// RETORNA: vacio
void CLogger::error(std::string msg) {

	if(level >= l_error)
		setMessage(getStrLevel(l_error), msg);
}

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'warn'
//
// RETORNA: vacio
void CLogger::warn(std::string msg) {

	if(level >= l_warn)
		setMessage(getStrLevel(l_warn), msg);
}

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'info'
//
// RETORNA: vacio
void CLogger::info(std::string msg) {

	if(level >= l_info)
		setMessage(getStrLevel(l_info), msg);
}

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'debug'
//
// RETORNA: vacio
void CLogger::debug(std::string msg) {

	if(level >= l_debug)
		setMessage(getStrLevel(l_debug), msg);
}

// RECIBE:
//
//	msg - Un std::string que representa el mensaje a escribir en el Logger
//
// REALIZA: La escritura del mensaje con nivel 'trace'
//
// RETORNA: vacio
void CLogger::trace(std::string msg) {

	if(level >= l_trace)
		setMessage(getStrLevel(l_trace), msg);
}

// RECIBE:
//
//	level - Una referencia a std::string que representa el nivel de escritura del mensaje (fatal, error, etc...)
//	msg - Una referencia a std::string que representa el mensaje de log a escribir
//
// REALIZA: La escritura del mensaje en cada uno de los appenders del logger
//
// RETORNA: Vacio
void CLogger::setMessage(std::string& level, std::string& msg) {

	timeval ahora;

	gettimeofday(&ahora, NULL);

	escribiendo.lock();

	for(unsigned char i = 0 ; i < linkedAppenders.size() ; i++)
		linkedAppenders[i]->write(msg, name, &ahora, level);

	escribiendo.unlock();
}

// RECIBE:
//
//	name Nombre de la instancia de CLogger a obtener
//
// REALIZA: Obtiene la instancia por nombre
//
// RETORNA: Un puntero a la instancia demandada si existe, NULL en otro caso
CLogger* CLogger::getLogger(std::string name) {

	if(mapLogger->find(name) != mapLogger->end())
		return (*mapLogger)[name];
	return NULL;
}

// RECIBE:
//
//	fileName - El nombre de un fichero XML a partir del cual se leera la configuracion
//
// REALIZA: Configura el logging
//
// RETORNA: True si la configuracion se ha podido realizar, False en otro caso
bool CLogger::configure(std::string fileName) {

	clear();

	strSAXError.str("");

	try {
                xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {

		char* message = xercesc::XMLString::transcode(toCatch.getMessage());

		strSAXError << "Excepcion en inicializacion: [" << message << "]";

                xercesc::XMLString::release(&message);
		return false;
	}

        xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
	parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);   // optional

	mapLogger = new mapOfLoggers();
	mapAppender = new mapOfAppenders();

	CSAX2Handler* handler = new CSAX2Handler(mapLogger, mapAppender, &strSAXError);

	parser->setContentHandler(handler);
	parser->setErrorHandler(handler);

	try {
		parser->parse(fileName.c_str());
	}
	catch (const xercesc::XMLException& toCatch) {

		char* message = xercesc::XMLString::transcode(toCatch.getMessage());

		strSAXError << "Excepcion XML parseando fichero: [" << fileName << "] [" << message << "]";

		xercesc::XMLString::release(&message);
		return false;
	}
	catch (const xercesc::SAXParseException& toCatch) {

		char* message = xercesc::XMLString::transcode(toCatch.getMessage());

		strSAXError << "Excepcion SAX parseando fichero: [" << fileName << "] [" << message << "]";

		xercesc::XMLString::release(&message);
		return false;
	}
	catch (...) {

		strSAXError << "Excepcion inesperada parseando fichero: [" << fileName << "]";
		return false;
	}

	delete parser;
	delete handler;

        xercesc::XMLPlatformUtils::Terminate();

	return true;
}

// RECIBE:
//
//	Vacio
//
// REALIZA: Poca cosa
//
// RETORNA: La cadena que contiene un posible error de parseo
//
// NOTA...: Es muy importante que esta funcion no sea inline, puesto que hace referencia
//		a una variable estatica declarada en este mismo fichero de implementacion
std::string CLogger::getParseError() {
	return strSAXError.str();
};

// RECIBE:
//
//	Vacio
//
// REALIZA: Limpia los objetos estaticos
//
// RETORNA: Vacio
void CLogger::clear() {

	if(mapLogger != NULL) {

		if(mapLogger->size() > 0) {

			mapOfLoggers::iterator it;
			for(it = mapLogger->begin() ; it != mapLogger->end() ; it++) {
				delete it->second;
			}
			mapLogger->clear();
		}

		delete mapLogger;
		mapLogger = NULL;
	}

	if(mapAppender != NULL) {

		if(mapAppender->size() > 0) {

			mapOfAppenders::iterator it;
			for(it = mapAppender->begin() ; it != mapAppender->end() ; it++) {
				delete it->second;
			}
			mapAppender->clear();
		}

		delete mapAppender;
		mapAppender = NULL;
	}
}
