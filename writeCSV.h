#ifndef __write_h_
#define __write_h_
#include <stdio.h>
#include <stdlib.h>

/*
Crea un nuevo archivo
*/
FILE * newFile(char * name);
/*
Da nombre a las columnas del csv de q1
*/
void writeHeaderQ1(FILE * stream);
/*
Agrega una fila al csv q1
*/
void writeRowQ1(FILE * stream,char * name,char * memberTrips,char * casualTrips,char * allTrips);
/*
Da nombre a las columnas del csv de q2
*/
void writeHeaderQ2(FILE * stream);
/*
Agrega una fila al csv q2
*/
void writeRowQ2(FILE * stream,char * name,char * nameEnd,char * oldestDate);
/*
Da nombre a las columnas del csv de q3
*/
void writeHeaderQ3(FILE * stream);
/*
Agrega una fila al csv q3
*/
void writeRowQ3(FILE * stream,char * weekday,char * startedTrips,char * endedTrips);
/*
Da nombre a las columnas del csv de q4
*/
void writeHeaderQ4(FILE * stream);
/*
Agrega una fila al csv q4
*/
void writeRowQ4(FILE * stream,char * name,char * mostPopRouteEndStation,char * mostPopRouteTrips);
/*
Da nombre a las columnas del csv de q5
*/
void writeHeaderQ5(FILE * stream);
/*
Agrega una fila al csv q5
*/
void writeRowQ5(FILE * stream,char * month,char * loopsTop1St,char * loopsTop2St,char * loopsTop3St);
#endif