#ifndef __read_h_
#define __read_h_
#include "stationADT.h"

/* 
Define que el formato no es el de NYC, si en el makefile no se llamo a la flag -DFORMATNYC\=1
*/
#ifndef FORMATNYC
#define FORMATNYC 0
#endif

/* 
Define que el formato no es el de MON, si en el makefile no se llamo a la flag -DFORMATMON\=1
*/
#ifndef FORMATMON
#define FORMATMON 0
#endif

/*
En caso de formato NYC se asignan sus enums para luego poder separar las cadenas de texto segun este formato
*/
#if FORMATNYC
#define NYCBIKES_FORMAT_ENUM \
    dateStart, \
    startedId, \
    dateEnd, \
    endedId, \
    rideableType, \
    member, \
    lastElemBikes

#define NYCSTATIONS_FORMAT_ENUM \
    stationName, \
    latitude, \
    longitude, \
    idStation, \
    lastElmStations

enum NYCBIKES {
    NYCBIKES_FORMAT_ENUM
};

enum NYCSTATIONS {
    NYCSTATIONS_FORMAT_ENUM
};

/*
En caso de formato MON se asignan sus enums para luego poder separar las cadenas de texto segun este formato
*/
#elif FORMATMON
#define MONBIKES_FORMAT_ENUM \
    dateStart, \
    startedId, \
    dateEnd, \
    endedId, \
    member, \
    lastElemBikes

#define MONSTATIONS_FORMAT_ENUM \
    idStation, \
    stationName, \
    latitude, \
    longitude, \
    lastElmStations

enum MONBIKES {
    MONBIKES_FORMAT_ENUM
};

enum MONSTATIONS {
    MONSTATIONS_FORMAT_ENUM
};
#endif
/*
Los enums anteriores son modificables para poder poner otros formatos
*/


#define BIKES_FILE 1 //Indica que numero de argumento es el del path del archivo de alquileres (El primer argumento es el 1) 
#define STATION_FILE 2 //Indica que numero de argumento es el del path del archivo de estaciones (El primer argumento es el 1)
#define DELIM ";" //Separador de datos, cambiar si separa los datos con otros simbolos
#define DATE_DELIM "- :" //Separador de fechas, cambiar si separa los valores de la fecha con otros simbolos
#define DAYLIGHT_SAVING_TIME -1 //-1 No se sabe si aplica, 0 No aplica, 1 Aplica

/*
La funcion toma el arreglo de argumentos y la estructura ya iniciada y carga en ella todos los datos de los archivos pasados por parametros
*/
void readCSVFileStation(char const * argv[],stationsADT stations);
#endif