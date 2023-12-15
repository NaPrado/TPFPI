#ifndef __read_h_
#define __read_h_
#include "stationADT.h"

#ifndef FORMATNYC
#define FORMATNYC 0
#endif
#ifndef FORMATMON
#define FORMATMON 0
#endif

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
La funcion toma el arreglo de argumentos y la estructura ya iniciada y carga en ella todos los datos de los archivos pasados por parametros
*/
void readCSVFileStation(char const * argv[],stationsADT stations);
#endif