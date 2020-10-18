#include "CAppender.h"

// RECIBE:
//
//	timeStampFormat - Una cadena de texto que representa el TimeStampFormat
//
// REALIZA: Establece el formato del timestamp
//
// RETORNA: Vacio
void CAppender::setTimeStampFormat(char* timeStampFormat) {

	if(!((std::string)"ISO8601").compare(timeStampFormat))
		setTimeStampFormat(ISO8601);
	else if(!((std::string)"ISO8601_only_date").compare(timeStampFormat))
		setTimeStampFormat(ISO8601_only_date);
	else if(!((std::string)"ISO8601_only_time").compare(timeStampFormat))
		setTimeStampFormat(ISO8601_only_time);
	else if(!((std::string)"localFormat_only_date").compare(timeStampFormat))
		setTimeStampFormat(localFormat_only_date);
	else if(!((std::string)"localFormat_only_time").compare(timeStampFormat))
		setTimeStampFormat(localFormat_only_time);
	else // localFormat
		setTimeStampFormat(localFormat);
}

// RECIBE:
//
//	now - Un puntero a la estructura timeval que representa el instante actual
//
// REALIZA: Construye el timestamp y lo formatea segun opcion
//
// RETORNA: La cadena que representa el timestamp formateado
std::string CAppender::getTimeStamp(timeval *now) const {

        std::ostringstream strDate;
	struct tm* date = localtime(&now->tv_sec);

	strDate << std::setfill('0');
	switch(timeStampFormat) {

		case ISO8601:

			strDate << std::setw(4) 
				<< date->tm_year + 1900
			       	<< std::setw(2)
			       	<< date->tm_mon + 1
			       	<< std::setw(2)
			       	<< date->tm_mday
			       	<< 'T'
			       	<< std::setw(2)
			       	<< date->tm_hour
			       	<< std::setw(2)
			       	<< date->tm_min
			       	<< std::setw(2)
			       	<< date->tm_sec
			       	<< ','
			       	<< std::setw(3)
			       	<< now->tv_usec / 1000;
			break;

		case ISO8601_only_date:

			strDate << std::setw(4)
			       	<< date->tm_year + 1900
			       	<< std::setw(2)
			       	<< date->tm_mon + 1
			       	<< std::setw(2)
			       	<< date->tm_mday;
			break;

		case ISO8601_only_time:

			strDate << 'T'
			       	<< std::setw(2)
			       	<< date->tm_hour
			       	<< std::setw(2)
			       	<< date->tm_min
			       	<< std::setw(2)
			       	<< date->tm_sec
			       	<< ',' << std::setw(3)
			       	<< now->tv_usec / 1000;
			break;

		case localFormat_only_date:

			strDate << std::setw(2)
			       	<< date->tm_mday
			       	<< '/'
			       	<< std::setw(2)
			       	<< date->tm_mon + 1
			       	<< '/'
			       	<< date->tm_year + 1900;
			break;

		case localFormat_only_time:

			strDate << std::setw(2)
			       	<< date->tm_hour
			       	<< ':'
			       	<< std::setw(2)
			       	<< date->tm_min
			       	<< ':'
			       	<< std::setw(2)
			       	<< date->tm_sec
			       	<< ','
			       	<< std::setw(3)
			       	<< now->tv_usec / 1000;
			break;

		default:

			// Por defecto es "localFormat"
			strDate << std::setw(2)
			       	<< date->tm_mday
			       	<< '/'
			       	<< std::setw(2)
			       	<< date->tm_mon + 1
			       	<< '/'
			       	<< date->tm_year + 1900
			       	<< ' '
			       	<< std::setw(2)
			       	<< date->tm_hour
			       	<< ':'
			       	<< std::setw(2)
			       	<< date->tm_min
			       	<< ':'
			       	<< std::setw(2)
			       	<< date->tm_sec
			       	<< ','
			       	<< std::setw(3)
			       	<< now->tv_usec / 1000;
	}
	return strDate.str();
}

// RECIBE:
//
//	now - Un puntero a la estructura timeval que representa el instante actual
//	name - Nombre del logger
//	level - el nivel de logging asociado al mensaje
//
// REALIZA: Construye el prefijo del layout segun las opciones configuradas writeDate y writeLevel
//
// RETORNA: La cadena que representa el prefijo del layout
std::string CAppender::getLayoutPrefix(timeval *now, std::string& name, std::string& level) const {

	std::string strPrefijo;

	if(writeDate)
		strPrefijo = getTimeStamp(now) + ' ';
	if(writeName)
		strPrefijo += '(' + name + ") ";
	if(writeLevel)
		strPrefijo += level + " - ";

	return strPrefijo;
}
