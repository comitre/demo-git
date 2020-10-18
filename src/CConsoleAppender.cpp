#include "CConsoleAppender.h"

// RECIBE:
//
//	msg - Cadena a escribir en consola
//	now - El instante del suceso a registrar
//	level - Un caracter que representa el nivel de logging
//
// REALIZA: Construye la cadena que se escribira por consola en funcion del layout
//
// RETORNA: Vacio
void CConsoleAppender::write(std::string& msg, std::string& name, timeval* now, std::string& level) {

	// Segun el layout se construye la cadena a escribir

	std::string message = getLayoutPrefix(now, name, level) + msg + '\n';

	outputMutex.lock();
        std::cout << message;
	outputMutex.unlock();
}
