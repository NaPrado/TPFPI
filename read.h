#ifndef __read_h_
#define __read_h_
#include "stationADT.h"

#ifndef FORMATNYC
#define FORMATNYC 0
#endif
#ifndef FORMATMON
#define FORMATMON 1
#endif

#if FORMATNYC
#define NYCBIKES_FORMAT_ENUM \
    dateStart, \
    startedId, \
    dateEnd, \
    endedId, \
    rideableType, \
    member

#define NYCSTATIONS_FORMAT_ENUM \
    stationName, \
    latitude, \
    longitude, \
    idStation

enum NYCBIKES {
    NYCBIKES_FORMAT_ENUM
};

enum NYCSTATIONS {
    NYCSTATIONS_FORMAT_ENUM
};

#define MAX_COLS_ELEMS_BIKES 6

#elif FORMATMON
#define MONBIKES_FORMAT_ENUM \
    dateStart, \
    startedId, \
    dateEnd, \
    endedId, \
    member

#define MONSTATIONS_FORMAT_ENUM \
    idStation, \
    stationName, \
    latitude, \
    longitude

enum MONBIKES {
    MONBIKES_FORMAT_ENUM
};

enum MONSTATIONS {
    MONSTATIONS_FORMAT_ENUM
};

#define MAX_COLS_ELEMS_BIKES 6

#endif

/*
La funcion toma el arreglo de argumentos y la estructura ya iniciada y carga en ella todos los datos de los archivos pasados por parametros
*/
void readCSVFileStation(char const * argv[],stationsADT stations);
#endif