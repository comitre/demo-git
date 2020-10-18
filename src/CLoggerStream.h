/******************************************************************************************
 **
 **					DPTO. SOFTWARE, AREA DE INGENIERIA, SISTEMSA
 **
 *******************************************************************************************

 *******************************************************************************************
 ** Identificador proyecto ....: logging
 ** Nombre fichero.............: LoggerStream.h
 ** Autor (es) ................: AFL
 ** Referencia ................:
 ** Descripción ...............: Fichero de cabecera de la clase CLoggerStream, utilizada para
 **				dotar a los logger de capacidades similares a los streams standar de c++
 **				(enviar cadenas formateadas, uso de manipuladores standar, etc...) 
 ** Fecha creacion ............: 20/12/2010
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20101220120000
 **
 ** Fecha modificacion ........: 30/05/2010
 ** Motivo modificacion .......: Problema de interbloqueo con mensajeria Windows en los operadores de inserción
 **				se soluciona usando objetos de tipo "mutex" en lugar de "critical_section" y la 
 **				funcion de sincronizacion "WaitForSingleObject" con timeout.
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20110530180000
 *******************************************************************************************/

#ifndef CLOGGERSTREAM_H_
#define CLOGGERSTREAM_H_

#include <sstream>
#include <string>
#include <mutex>

// Declaracion forward
class CLogger;

class CLoggerStream {

	public:

		CLoggerStream();
		~CLoggerStream(void) {};

		/* Sobrecarga del operador de insercion mediante plantilla generica para soportar
		 * la insercion de todos los tipos de dato */
		template <class T>
			CLoggerStream& operator<<(T const& value) {

				mutex.lock();
				// Se incrementa la cuenta de bloqueos del mutex recursivo
				bloqueos++;
				str << value;

				return *this;
			};

		/* Sobrecarga del operador de insercion particularizada para el tipo de dato char*
		 * permitira detectar el codigo de fin de insercion "end()" para volcado y vaciado del buffer */
		CLoggerStream& operator<<(char const* value);

		// Funcion de asignacion del objeto CLogger a utilizar para el volcado del buffer
		void setLogger(CLogger* logger) {this->logger = logger;};

		// Funcion de asignacion de la funcion a utilizar del objeto "CLogger" para el volcado del buffer
		void setDumpFunction(void (CLogger::*dumpFunction)(std::string)) {
			this->dumpFunction = dumpFunction;
		}

		// Codigo de fin de insercion
		static const char* const end() {return "@#$$#@";};	

	private:

		// Buffer donde se ira almacenando el stream
		std::ostringstream str;

		// Puntero al logger relacionado con el stream
		CLogger* logger;
		// Puntero a la funcion del logger que se invocara para volcar el buffer.
		void (CLogger::*dumpFunction)(std::string);

		// Variables para dotar a la clase de seguridad multithreading
		std::recursive_mutex mutex;

		/* Contador de bloqueos de la seccion critica. Por cada llamada a "EnterCriticalSection"
		 * es necesario hacer una llamada a "LeaveCriticalSection", para esto se precisa llevar la cuenta */
		unsigned short bloqueos;
};

#endif
