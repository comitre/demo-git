OBJECTS = CAppender.o CConsoleAppender.o CFileAppender.o CRollingFileAppender.o CRollingSizeFileAppender.o CRollingTimeFileAppender.o CSizeFileAppender.o CMemoryAppender.o CLoggerStream.o CLogger.o CSAX2Handler.o

INCLUDES = /home/adrian/programacion/c++/librerias

# Emplear el compilador de GNU-
CXX = g++
# Compilacion con el estandar c++ 17 (hay que incluir tambien la opcion -lstdc++fs)
CXXFLAGS = -fPIC -std=c++17 -I$(INCLUDES) -lstdc++fs

RUTA = ~/programacion/c++/librerias/lib/
NAME = liblogging
VERSION = 2.1.0

LIBNAME = $(RUTA)$(NAME)$(VERSION)

crearLibrerias: $(OBJECTS)
	ar -crs $(LIBNAME).a $(OBJECTS)
	${CXX} -shared -o $(LIBNAME).so $(OBJECTS)

CAppender.o: CAppender.h
CConsoleAppender.o: CConsoleAppender.h CAppender.h
CFileAppender.o: CFileAppender.h CAppender.h
CRollingFileAppender.o: CRollingFileAppender.h CFileAppender.h
CRollingSizeFileAppender.o: CRollingSizeFileAppender.h CRollingFileAppender.h
CRollingTimeFileAppender.o: CRollingTimeFileAppender.h CRollingFileAppender.h
CSizeFileAppender.o: CSizeFileAppender.h CFileAppender.h
CMemoryAppender.o: CMemoryAppender.h CAppender.h
CLoggerStream.o: CLoggerStream.h
CLogger.o: CLogger.h CAppender.h 
CSAX2Handler.o: CSAX2Handler.h

clean:
	#rm -f $(LIBNAME).a $(LIBNAME).so $(OBJECTS)
	rm -f $(OBJECTS)
