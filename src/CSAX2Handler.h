/******************************************************************************************
 **
 **					DPTO. SOFTWARE, AREA DE INGENIERIA, SISTEMSA
 **
 *******************************************************************************************

/******************************************************************************************
 **
 ** Identificador proyecto ....: logging
 ** Nombre fichero.............: SAX2Handler.h
 ** Autor (es) ................: AFL
 ** Referencia ................:
 ** Descripción ...............: Fichero de cabecera de la clase CSAX2Handler, utilizada para
 **				 procesar adecuadamente el fichero XML de definicion del logging
 ** Fecha creacion ............: 13/12/2010
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20101213120000
 **
 *******************************************************************************************/


#ifndef C_SAX2HANDLER_H_
#define C_SAX2HANDLER_H_

#include "CLogger.h"

#include "CConsoleAppender.h"
#include "CMemoryAppender.h"
#include "CRollingSizeFileAppender.h"
#include "CRollingTimeFileAppender.h"
#include "CSizeFileAppender.h"

#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

class CSAX2Handler : public xercesc::DefaultHandler
{
	public:

		CSAX2Handler(CLogger::mapOfLoggers*, CLogger::mapOfAppenders*, std::ostringstream*);
		~CSAX2Handler(void){};

		void setMapLogger(CLogger::mapOfLoggers* mapLogger) {this->mapLogger = mapLogger;};
		void setMapAppender(CLogger::mapOfAppenders* mapAppender) {this->mapAppender = mapAppender;};

		void startElement(const XMLCh* const uri,
                                  const XMLCh* const localname,
                                  const XMLCh* const qname,
                                  const xercesc::Attributes& attrs);

		void endElement(const XMLCh *const uri,
                                const XMLCh *const localname,
                                const XMLCh *const qname);

		void fatalError(const xercesc::SAXParseException&);

	private:

		CLogger* logger;
		CAppender* appender;
		std::ostringstream* strError;

		CLogger::mapOfLoggers* mapLogger;
		CLogger::mapOfAppenders* mapAppender;

		void processLabelLogger(const xercesc::Attributes& attrs);
		void processLabelLevel(const xercesc::Attributes& attrs);
		void processLabelAppenderRef(const xercesc::Attributes& attrs);
		void processLabelAppender(const xercesc::Attributes& attrs);
		void processLabelParam(const xercesc::Attributes& attrs);
};

#endif /* C_SAX2HANDLER_H_ */
