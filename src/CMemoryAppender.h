/******************************************************************************************
 **
 **					DPTO. SOFTWARE, AREA DE INGENIERIA, SISTEMSA
 **
 *******************************************************************************************

/******************************************************************************************
 **
 ** Identificador proyecto ....: logging
 ** Nombre fichero.............: MemoryAppender.h
 ** Autor (es) ................: AFL
 ** Referencia ................:
 ** Descripción ...............: Fichero de cabecera de la clase CMemoryAppender
 ** Fecha creacion ............: 25/03/2010
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20100426120000
 **
 ** Fecha modificacion ........: 10/11/2010
 ** Motivo modificacion .......: Se añade funcionalidad para obtener los identificadores de version y compilacion
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.1
 ** Identificador compilacion .:	20101110100000
 **
 ** Fecha modificacion ........: 13/12/2010
 ** Motivo modificacion .......: Necesidad de implementar nuevos metodos para la integracion del mecanismo
 **								de configuracion de logging a partir de fichero XML.
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.2
 ** Identificador compilacion .: 20101213120000
 **
 *******************************************************************************************/

#pragma once

#include "CAppender.h"
#include <iostream> 
#include <queue>
#include <mutex>

#define MIN_QUEUE_SIZE 10	// Tamaño minimo permitido para la cola de registros en memoria
#define MAX_QUEUE_SIZE 100000	// Tamaño maximo permitido para la cola de registros en memoria

class CMemoryAppender : public CAppender {

	public:

		CMemoryAppender(unsigned int maxQueueSize = MIN_QUEUE_SIZE, CAppender* appender = NULL);
		virtual ~CMemoryAppender() {};

		void setMaxQueueSize(unsigned int maxQueueSize);
		void setMaxQueueSize(char* maxQueueSize) {setMaxQueueSize(atoi(maxQueueSize));};

		unsigned int getMaxQueueSize() const {return maxQueueSize;};

		void setAppender(CAppender* appender);
		CAppender* getAppender() const {return appender;};

		void write(std::string& msg, std::string& name, timeval* now, std::string& level);

	private:

		struct message {

			std::string msg;
			std::string name;
			timeval date;
			std::string level;
		};

		CAppender* appender;
		unsigned int maxQueueSize;
                std::queue<message>messageQueue;

                std::mutex seccionCriticaCola;
};
