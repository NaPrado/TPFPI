#ifndef __write_h_
#define __write_h_
#include <stdio.h>
#include <stdlib.h>
// este es el separador de datos
#define DELIM ";"
/*
Crea un nuevo archivo de formato .csv
*/
FILE * newFile(char * name);
/*
Da nombre a las columnas del csv para la query 1
*/
void writeHeaderQ1(FILE * stream);
/*
Agrega una fila al csv para la query 1
*/
void writeRowQ1(FILE * stream,char * name,char * memberTrips,char * casualTrips,char * allTrips);
/*
Da nombre a las columnas del csv para la query 2
*/
void writeHeaderQ2(FILE * stream);
/*
Agrega una fila al csv para la query 2
*/
void writeRowQ2(FILE * stream,char * name,char * nameEnd,char * oldestDate);
/*
Da nombre a las columnas del csv para la query 3
*/
void writeHeaderQ3(FILE * stream);
/*
Agrega una fila al csv para la query 3
*/
void writeRowQ3(FILE * stream,char * weekday,char * startedTrips,char * endedTrips);
/*
Da nombre a las columnas del csv para la query 4
*/
void writeHeaderQ4(FILE * stream);
/*
Agrega una fila al csv para la query 4
*/
void writeRowQ4(FILE * stream,char * name,char * mostPopRouteEndStation,char * mostPopRouteTrips);
#endif