#ifndef __read_h_
#define __read_h_
#include "stationADT.h"

/*
La funcion toma el arreglo de argumentos y la estructura ya iniciada y carga en ella todos los datos de los archivos pasados por parametros 
en formato Montreal
*/
void inicializerMONFormat(char const * argv[],stationsADT stations);
/*
La funcion toma el arreglo de argumentos y la estructura ya iniciada y carga en ella todos los datos de los archivos pasados por parametros 
en formato Nueva York
*/
void inicializerNYCFormat(char const * argv[],stationsADT stations);
#endif