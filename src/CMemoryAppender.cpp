/******************************************************************************************
 **
 ** Identificador proyecto ....: logging
 ** Nombre fichero.............: MemoryAppender.cpp
 ** Autor (es) ................: AFL
 ** Referencia ................:
 ** Descripción ...............: Fichero de implementacion de la clase CMemoryAppender
 ** Fecha creacion ............: 25/03/2010
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20100426120000
 **
 ******************************************************************************************
 **
 ** Fecha modificacion ........: 10/11/2010
 ** Motivo modificacion .......: (Ver fichero de cabecera)
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.1
 ** Identificador compilacion .: 20101110100000
 **
 ******************************************************************************************
 **
 ** Fecha modificacion ........: 13/12/2010
 ** Motivo modificacion .......: Necesidad de implementar nuevos metodos para la integracion del mecanismo
 **                              de configuracion de logging a partir de fichero XML.
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.2
 ** Identificador compilacion .: 20101213120000
 **
 ******************************************************************************************
 **
 ** Fecha modificacion ........: 01/11/2019
 ** Motivo modificacion .......: Revision general del codigo 
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.3
 ** Identificador compilacion .: 20191101120000
 *******************************************************************************************/

#include "CMemoryAppender.h"

// RECIBE: 
//
//	maxQueueSize - Numero maximo de registros en memoria antes de volcarlos al appender auxiliar
//	appender - Un puntero al appender en el que se volcara el contenido de la memoria
//
// REALIZA: La construccion de un objeto CMemoryAppender
//
// RETORNA: Vacio
CMemoryAppender::CMemoryAppender(unsigned int maxQueueSize, CAppender* appender) {

	setMaxQueueSize(maxQueueSize);
	setAppender(appender);
}

// RECIBE:
//
//	maxQueueSize - El tamaño maximo a establecer a la cola de registros en memoria
//
// REALIZA: Asegura que el valor este dentro de los limites y lo asigna al miembro
//
// RETORNA: Vacio
void CMemoryAppender::setMaxQueueSize(unsigned int maxQueueSize) {

	// Se comprueba que el tamaño maximo este dentro de los limites permitidos
	if(maxQueueSize < MIN_QUEUE_SIZE)
		maxQueueSize = MIN_QUEUE_SIZE;
	else if(maxQueueSize > MAX_QUEUE_SIZE)
		maxQueueSize = MAX_QUEUE_SIZE;

	this->maxQueueSize = maxQueueSize;
}

// RECIBE:
//
//	appender - Un puntero al appender auxiliar en el que se volcara el contenido de la memoria
//
// REALIZA: Asigna el appender si no es del tipo CMemoryAppender. En caso contrario asigna NULL
//
// RETORNA: Vacio
void CMemoryAppender::setAppender(CAppender* appender) {

	if(appender != NULL && typeid(*appender).name() != typeid(*this).name())
		this->appender = appender;
	else
		this->appender = NULL;
};

// RECIBE:
//
//	msg - Cadena a escribir en el appender
//	name - Nombre del logger
//	now - El instante del suceso a registrar
//	level - Cadena que representa el nivel de logging
//
// REALIZA: Inserta en la cola de mensajes la cadena, la fecha y el nivel de log, mientras el tamaño lo permita.
//		Cuando se supera el tamaño maximo definido se vuelca el contenido de la cola al appender auxiliar
//
// RETORNA: Vacio
void CMemoryAppender::write(std::string& msg, std::string& name, timeval* now, std::string& level) {

	if(appender != NULL) {

		message m;

		seccionCriticaCola.lock();

		if(messageQueue.size() >= maxQueueSize) {

			std::string str;

			/* El appender asociado puede tener valores diferentes para los parametros de "layout" por tanto
			 * antes de hacer el volcado en dicho appender hay que establecerle temporalmente los parametros
			 * de "layout" del "MemoryAppender" y restablecer luego de nuevo los anteriores */
			bool writeDateTmp, writeNameTmp, writeLevelTmp;
			enum timeStampFormat timeStampFormatTmp;

			// Se guardan los parametros de "layout" del appender auxiliar (para restablecerlos luego)
			timeStampFormatTmp = appender->getTimeStampFormat();
			writeDateTmp = appender->getWriteDate();
			writeNameTmp = appender->getWriteName();
			writeLevelTmp = appender->getWriteLevel();

			// Se le asigna al appender auxiliar los parametros de "layout" de este appender
			appender->setTimeStampFormat(getTimeStampFormat());
			appender->setWriteDate(getWriteDate());
			appender->setWriteName(getWriteName());
			appender->setWriteLevel(getWriteLevel());

			str = "------- Volcado de [";
			str.append(getName());
			str.append("] en [");
			str.append(appender->getName());
			str.append("] de ");
			str.append(std::to_string(messageQueue.size()));
			str.append(" registros de log en memoria.......");
			appender->write(str, name, now, level);

			while(!messageQueue.empty()) {

				m = messageQueue.front();
				messageQueue.pop();
				appender->write(m.msg, m.name, &m.date, m.level);
			}

			// Restablecimiento de los parametros de "layout" originales del appender auxiliar
			appender->setTimeStampFormat(timeStampFormatTmp);
			appender->setWriteDate(writeDateTmp);
			appender->setWriteName(writeNameTmp);
			appender->setWriteLevel(writeLevelTmp);
		}

		m.msg = msg;
		m.name = name;
		m.date = *now;
		m.level = level;

		messageQueue.push(m);

		seccionCriticaCola.unlock();
	}
}
