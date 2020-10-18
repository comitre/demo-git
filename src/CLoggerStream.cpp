/*******************************************************************************************
 ** Identificador proyecto ....: logging
 ** Nombre fichero.............: LoggerStream.h
 ** Autor (es) ................: AFL
 ** Referencia ................:
 ** Descripción ...............: Fichero de implementacion de la clase CLoggerStream
 ** Fecha creacion ............: 20/12/2010
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20101220120000
 **
 *******************************************************************************************
 **
 ** Fecha modificacion ........: 30/05/2010
 ** Motivo modificacion .......: Problema de interbloqueo con mensajeria Windows en los operadores de inserción
 **				 se soluciona usando objetos de tipo "mutex" en lugar de "critical_section" y la 
 **				 funcion de sincronizacion "WaitForSingleObject" con timeout.
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.0
 ** Identificador compilacion .: 20110530180000
 **
 *******************************************************************************************
 **
 ** Fecha modificacion ........: 01/11/2019
 ** Motivo modificacion .......: Revision general del codigo
 ** Autor modificacion ........: AFL
 ** Identificador version .....: 1.1
 ** Identificador compilacion .: 20191101120000
 **
 *******************************************************************************************/

#include "CLoggerStream.h"
#include <string.h>

// RECIBE:
//
//	Vacio
//
// REALIZA: Construye un objeto de tipo CLoggerStream. Establece a NULL los punteros al logger relacionado
//		la funcion de volcado del stream e inicializa el objeto para la gestion de secciones criticas
//
// RETORNA: vacio
CLoggerStream::CLoggerStream() {

	logger = NULL;
	dumpFunction = NULL;

	// Inicializacion del mutex (Se utilizara un mutex recursivo)
	bloqueos = 0;
};

// RECIBE:
//
//	Value
//
// REALIZA: En caso de que el valor recibido sea el codigo de finalizacion de insercion vuelca el contenido
//		del buffer en la funcion de volcado, lo vacia y libera la seccion critica. En otro caso
//		inserta la cadena de texto recibida en el buffer.
//
// RETORNA: Una referencia al propio "CLoggerStream" (esto permite el encadenamiento de operadores)
CLoggerStream& CLoggerStream::operator<<(char const* value) {

	mutex.lock();

	// Se incrementa la cuenta de bloqueos del mutex 
	bloqueos++;

	if(!strcmp(value, end())) {

		// Se ha llegado al punto en que se debe hacer el volcado del buffer y liberar la seccion critica

		if(logger != NULL && dumpFunction != NULL)
			(logger->*dumpFunction)(str.str());

		str.str("");

		/* Es necesario hacer una llamada a "unlock" por cada una de las llamadas
		 * a "lock". Para ello se ha llevado la cuenta en la vble "bloqueos".
		 * 
		 * Se iterara un numero de veces igual a "bloqueos" haciendo llamadas a "unlock".
		 * Es muy importante que en la condicion de parada del bucle no intervenga directamente
		 * la vble "bloqueos", ya que despues del ultimo "unlock", esta vble puede ser 
		 * incrementada por otro "thread" haciendo de nuevo "falsa" a la condicion de parada, provocando
		 * resultados no deseados. Por esto se utiliza la vble auxiliar "bloqueosTmp"
		 */

		unsigned short bloqueosTmp = bloqueos;
		bloqueos = 0;

		while(bloqueosTmp > 0) {

			mutex.unlock();
			bloqueosTmp --;
		}

	} else // No es el fin del encadenamiento de operadores
		str << value;

	return *this;
}
