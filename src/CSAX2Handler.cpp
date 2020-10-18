/******************************************************************************************
 **
 **					DPTO. SOFTWARE, AREA DE INGENIERIA, SISTEMSA
 **
 *******************************************************************************************

/******************************************************************************************
 **
 ** Identificador proyecto ....: logging
 ** Nombre fichero.............: SAX2Handler.cpp
 ** Autor (es) ................: AFL
 ** Referencia ................:
 ** Descripción ...............: Fichero de implementacion de la clase CSAX2Handler, utilizada para
 **                              procesar adecuadamente el fichero XML de definicion del logging
 ** Fecha creacion ............: 13/12/2010
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20101213120000
 **
 *******************************************************************************************/

#include "CSAX2Handler.h"

// RECIBE:
//
//	mapLogger Un puntero al mapa global de objetos CLogger
//	mapAppender Un puntero al mapa global de punteros a objeto CAppender
//	strError Un puntero a una cadena para almacenar posibles errores de parseo
//
// REALIZA: Construye un objeto de tipo CSAX2Handler
//
// RETORNA: Vacio
CSAX2Handler::CSAX2Handler(CLogger::mapOfLoggers* mapLogger, 
                           CLogger::mapOfAppenders* mapAppender,
                           std::ostringstream* strError) {

	setMapLogger(mapLogger);
	setMapAppender(mapAppender);
	this->strError = strError;
	logger = NULL;
	appender = NULL;
}

// RECIBE:
//
//	uri - URI de la etiqueta XML
//	localname - Nombre de la etiqueta XML
//	qname - qname de la etiqueta XML
//	attrs - Referencia a los atributos de la etiqueta XML
//
// REALIZA: Es la funcion llamada ante el evento de deteccion de una etiqueta XML
//
// RETORNA: Vacio
void CSAX2Handler::startElement(const XMLCh* const uri,
                                const XMLCh* const localname,
                                const XMLCh* const qname,
                                const xercesc::Attributes& attrs) {

	char* msg = xercesc::XMLString::transcode(localname);

	if(!strcmp(msg, "logger"))
		processLabelLogger(attrs);
	else if(!strcmp(msg, "level"))
		processLabelLevel(attrs);
	else if(!strcmp(msg, "appender-ref"))
		processLabelAppenderRef(attrs);
	else if(!strcmp(msg, "appender"))
		processLabelAppender(attrs);
	else if(!strcmp(msg, "param"))
		processLabelParam(attrs);

	xercesc::XMLString::release(&msg);
}

// RECIBE:
//
//	uri - URI de la etiqueta XML
//	localname - Nombre de la etiqueta XML
//	qname - qname de la etiqueta XML
//
// REALIZA: Es la funcion llamada ante el evento de deteccion de final de una etiqueta XML
//
// RETORNA: vacio
void CSAX2Handler::endElement(const XMLCh *const uri,
                              const XMLCh *const localname,
                              const XMLCh *const qname) {

	char* label = xercesc::XMLString::transcode(localname);

	if(!strcmp(label, "logger"))
		logger = NULL;
	else if(!strcmp(label, "appender"))
		appender = NULL;

	xercesc::XMLString::release(&label);
}

// RECIBE:
//
//	exception - Una referencia a un objeto SAXParseException (para control de errores de parseo)
//
// REALIZA: Es la funcion llamada ante un evento de error de parseo
//
// RETORNA: Vacio
void CSAX2Handler::fatalError(const xercesc::SAXParseException& exception) {

	char* message = xercesc::XMLString::transcode(exception.getMessage());

	(*strError) << "Error: " << message << " en linea: " << exception.getLineNumber() << std::endl;

	xercesc::XMLString::release(&message);
}

// RECIBE:
//
//	attrs - Referencia al objeto con los atributos de la etiqueta XML
//
// REALIZA: Procesa la etiqueta "logger"
//
// RETORNA: Vacio
void CSAX2Handler::processLabelLogger(const xercesc::Attributes& attrs) {

	if(NULL == mapLogger) return;

	char* msg;

	for(XMLSize_t index = 0 ; index < attrs.getLength() ; index++) {

		msg = xercesc::XMLString::transcode(attrs.getLocalName(index));

		if(!strcmp(msg, "name")) {

			// Obtener el valor del nombre
			xercesc::XMLString::release(&msg);
			msg = xercesc::XMLString::transcode(attrs.getValue(index));

			logger = CLogger::getLogger(msg);

			// Si el logger no existe, se crea. En otro caso no se hace nada
			if(NULL == logger) {

				logger = new CLogger();
				logger->setName(msg);

				(*mapLogger)[logger->getName()] = logger;
			}
		}
		xercesc::XMLString::release(&msg);
	}
}

// RECIBE:
//
//	attrs - Referencia al objeto con los atributos de la etiqueta XML
//
// REALIZA: Procesa la etiqueta "level" de logger en curso
//
// RETORNA: Vacio
void CSAX2Handler::processLabelLevel(const xercesc::Attributes& attrs) {

	if(NULL == logger) return;

	char* msg;
	for(XMLSize_t index = 0 ; index < attrs.getLength() ; index++) {

		msg = xercesc::XMLString::transcode(attrs.getLocalName(index));

		if(!strcmp(msg, "value")) {

			// Obtener el valor del nombre
			xercesc::XMLString::release(&msg);
			msg = xercesc::XMLString::transcode(attrs.getValue(index));
			logger->setLevel(msg);
		}
		xercesc::XMLString::release(&msg);
	}
}

// RECIBE:
//
//	attrs - Referencia al objeto con los atributos de la etiqueta XML
//
// REALIZA: Procesa la etiqueta "appender-ref"
//
// RETORNA: Vacio
void CSAX2Handler::processLabelAppenderRef(const xercesc::Attributes& attrs) {

	if(NULL == logger) return;

	char* msg;

	for(XMLSize_t index = 0 ; index < attrs.getLength() ; index++) {

		msg = xercesc::XMLString::transcode(attrs.getLocalName(index));

		if(!strcmp(msg, "ref")) {

			// Obtener el valor del nombre
			xercesc::XMLString::release(&msg);
			msg = xercesc::XMLString::transcode(attrs.getValue(index));

			// Se comprueba si el appender existe y en caso afirmativo se asigna
			if(mapAppender->find(msg) != mapAppender->end())
				logger->addAppender((*mapAppender)[msg]);
		}

		xercesc::XMLString::release(&msg);
	}
}

// RECIBE:
//
//	attrs - Referencia al objeto xercesc::Attributes de la etiqueta XML
//
// REALIZA: procesa los atributos de la etiqueta "Appender" en curso
//
// RETORNA: Vacio
void CSAX2Handler::processLabelAppender(const xercesc::Attributes& attrs) {

	if(NULL == mapAppender) return;

	char *msg = NULL, *nombre = NULL, *clase = NULL;

	for(XMLSize_t index = 0 ; index < attrs.getLength() ; index++) {

		msg = xercesc::XMLString::transcode(attrs.getLocalName(index));

		if(!strcmp(msg, "name"))

			// Obtencion del valor del nombre del appender
			nombre = xercesc::XMLString::transcode(attrs.getValue(index));

		else if(!strcmp(msg, "class"))

			// Obtencion del valor de la clase del appender
			clase = xercesc::XMLString::transcode(attrs.getValue(index));

		xercesc::XMLString::release(&msg);
	}

	// Si el nombre es NULL o existe un appender en el map con ese mismo nombre no se continua
	if(NULL == nombre || mapAppender->find(nombre) != mapAppender->end()) {

		if(nombre != NULL)
		       	xercesc::XMLString::release(&nombre);
		if(clase != NULL)
			xercesc::XMLString::release(&clase);
		return;
	}

	if(clase != NULL) {

		if(!strcmp(clase, "CMemoryAppender"))
			appender = new CMemoryAppender();
		else if(!strcmp(clase, "CRollingSizeFileAppender"))
			appender = new CRollingSizeFileAppender();
		else if(!strcmp(clase, "CRollingTimeFileAppender"))
			appender = new CRollingTimeFileAppender();
		else if(!strcmp(clase, "CSizeFileAppender"))
			appender = new CSizeFileAppender();
		else if(!strcmp(clase, "CConsoleAppender"))
			appender = new CConsoleAppender();

		xercesc::XMLString::release(&clase);

		if(appender != NULL) {
			appender->setName(nombre);
			(*mapAppender)[appender->getName()] = appender;
		}
	}

	xercesc::XMLString::release(&nombre);
}

// RECIBE:
//
//	attrs - Referencia al objeto con los atributos de la etiqueta XML
//
// REALIZA: Procesa la etiqueta "param" del appender en curso
//
// RETORNA: Vacio
void CSAX2Handler::processLabelParam(const xercesc::Attributes& attrs) {

	if(NULL == appender) return;

	char *msg = NULL, *nombre = NULL, *valor = NULL;

	for(XMLSize_t index = 0 ; index < attrs.getLength() ; index++) {

		msg = xercesc::XMLString::transcode(attrs.getLocalName(index));

		if(!strcmp(msg, "name"))

			// Obtencion del valor del nombre del parametro
			nombre = xercesc::XMLString::transcode(attrs.getValue(index));

		else if(!strcmp(msg, "value"))

			// Obtencion del valor del parametro
			valor = xercesc::XMLString::transcode(attrs.getValue(index));

		xercesc::XMLString::release(&msg);
	}

	if(nombre != NULL) {

		if(valor != NULL) {

			// CAppender
			if(!strcmp(nombre, "TimeStampFormat"))

				appender->setTimeStampFormat(valor);

			else if(!strcmp(nombre, "WriteDate"))

				appender->setWriteDate(valor);

			else if(!strcmp(nombre, "WriteLevel"))

				appender->setWriteLevel(valor);

			else if(!strcmp(nombre, "WriteName"))

				appender->setWriteName(valor);

			// CMemoryAppender
			else if(!strcmp(nombre, "MaxQueueSize")) {

				if(CMemoryAppender* ma = dynamic_cast<CMemoryAppender*>(appender))
					ma->setMaxQueueSize(valor);

			} else if(!strcmp(nombre, "AppenderAux")) {

				if(CMemoryAppender* ma = dynamic_cast<CMemoryAppender*>(appender)) {
					if(mapAppender->find(valor) != mapAppender->end())
						ma->setAppender((*mapAppender)[valor]);
				}

			// CFileAppender
			} else if(!strcmp(nombre, "MaxFileSize")) {

				if(CFileAppender* fa = dynamic_cast<CFileAppender*>(appender))
					fa->setMaxFileSize(valor);

			} else if(!strcmp(nombre, "PathName")) {

				if(CFileAppender* fa = dynamic_cast<CFileAppender*>(appender))
					fa->setPathName(valor);

			} else if(!strcmp(nombre, "InitString")) {

				if(CFileAppender* fa = dynamic_cast<CFileAppender*>(appender))
					fa->addInitString(valor);

			// CRollingFileAppender
			} else if(!strcmp(nombre, "FilePrefix")) {

				if(CRollingFileAppender* rfa = dynamic_cast<CRollingFileAppender*>(appender))
					rfa->setFilePrefix(valor);

			} else if(!strcmp(nombre, "MaxNumFiles")) {

				if(CRollingFileAppender* rfa = dynamic_cast<CRollingFileAppender*>(appender))
					rfa->setMaxNumFiles(valor);

			// CRollingTimeFileAppender
			} else if(!strcmp(nombre, "RollingPeriod")) {

				if(CRollingTimeFileAppender* rtfa = dynamic_cast<CRollingTimeFileAppender*>(appender))
					rtfa->setRollingPeriod(valor);

			// CSizeFileAppender
			} else if(!strcmp(nombre, "FileName")) {

				if(CSizeFileAppender* sfa = dynamic_cast<CSizeFileAppender*>(appender))
					sfa->setFileName(valor);
			}

			xercesc::XMLString::release(&valor);
		}

		xercesc::XMLString::release(&nombre);
	}
}
