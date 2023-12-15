#ifndef __stationsADT_h_
#define __stationsADT_h_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DAYS_IN_WEEK 7
#define MONTHS_IN_YEAR 12

#define INDICATOR_HAS_NO_UPPER_LIMIT -1
#define INDICATOR_HAS_NO_LOWER_LIMIT -1
#define EMPTY_IDENTIFIER "Empty"
#define INVALIDATE_INTERVAL -2

typedef struct stationsCDT * stationsADT; 

/*
Se crea el ADT en donde se guardan los datos y se allocan todos los recursos que necesita. Recibe un intervalo cerrado definido en los  
argumentos para el funcionamiento de algunas funciones. Abortara en el caso que se hayan mandado incorrectamente los años. Se define incorrectamente
si: se reciben menos de dos argumentos (los archivos a leer), o mas de cuatro (lso dos archivos y los dos años). Si no se reciben años se asume que el intervalo
es la totalidad de los datos, si se recibe solo uno, el intervalo se define como todos los años a partir del recibido por argumento, y si se recibe dos,
ese es el intervalo con el q se trabaja. Tambien aborta si el segundo argumento es mayor al primero.
*/
stationsADT newStationsGroup(int argc, char const *argv[]);
/*
Añade al ADT una estación. Recibe al ADT, el nombre de la estacion, a un arbol inicializado y la Id de la misma.
*/
void addStation(stationsADT stations,char * stationsName, size_t stationsId);
/*
Añade un alquiler al ADT. Recibe al arbol, fecha de entrega, id de la estacion de entrega, fecha de devolución, 
id de la estación de devolución, tipo de asociasion (si es miembro o casual) y el ADT.
*/
void addRental(struct tm startDate,size_t startId,struct tm endDate, size_t endId, char association, stationsADT stations);
/*
Libera todos los recursos empleados por el ADT. Recibe al ADT que se quiera liberar.
*/
void freeAssets(stationsADT stations);

/* 
Se debe llamar a esta funcion para poder usar las otras funciones de Count
es un iniciador de iterador en el orden solicitado para Count
Si se lo llama 2 veces se reinicia
*/
void toBeginCount(stationsADT stations);
/*
Consulta si existe un proximo elemento para Count
*/
int hasNextCount(stationsADT stations);
/*
avanza el iterador de Count
*/
void nextCount(stationsADT stations);
/*
Obtiene la cantidad de alquileres hechos por miembros en la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
size_t getMembersCount(stationsADT stations);
/* 
Obtiene la cantidad de alquileres hechos por casuales en la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
size_t getCasualsCount(stationsADT stations);
/*
Obtiene la cantidad de alquileres hechos en la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
size_t getTotalCount(stationsADT stations);
/*
Obtiene el nombre de la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
char * getStationNameCount(stationsADT stations);
/* 
Se debe llamar a esta funcion para poder usar las otras funciones Alpha
es un iniciador de iterador Consulta si existe un proximo elemento por Alpha
Si se lo llama 2 veces se reinicia
*/
void toBeginAlpha(stationsADT stations);
/*
Consulta si existe un proximo elemento por Alpha
*/
int hasNextAlpha(stationsADT stations);
/*
avanza el iterador Alpha
*/
void nextAlpha(stationsADT stations);
/* 
Verifica si la estacion iterada por Alpha tiene algun alquiler, 
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
int hasRentsAlpha(stationsADT stations);
/*
obtiene el nombre de la estacion iterada por Alpha,
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
char * getStationNameAlpha(stationsADT stations);
/*
obtiene el nombre del destino del viaje mas antiguo desde la estacion iterada por Alpha,
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
char * getOldestRentalStationNameEndAlpha(stationsADT stations);
/*
obtiene la fecha de partida del viaje mas antiguo desde la estacion iterada por Alpha,
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
struct tm getOldestRentalStartDateAlpha(stationsADT stations);


/*
Retorna un vector de dim DAYS_IN_WEEK con la cantidad de viajes empezados por dia de la semana
siendo v[0]= a la cantidad de los lunes, v[1]= a la de los martes, etc..
*/
size_t * getStartedTrips(stationsADT stations);
/*
Retorna un vector de dim DAYS_IN_WEEK con la cantidad de viajes finalizados por dia de la semana
siendo v[0]= a la cantidad de los lunes, v[1]= a la de los martes, etc..
*/
size_t * getEndedTrips(stationsADT stations);
/*
Devuelve un string con el nombre de la estacion mas popular para aquella en la que el iterador alfabetico esta posicionado. Es responsabilidad del usuario
que el iterador este inicializado y avanzar por el mismo. Ademas recibe por parametro un puntero a la cantidad de alquileres que hayan terminado en la
estacion mas popular
*/
char * getMostPopularFromStationAlpha(stationsADT stations, size_t * amountOfTrips);






/*
La funcion recibe el mes en el que se le quiere averiguar cual es el top tres estaciones por cantidad de viajes circulares. Recibe como parametro 
un vector de dimension tres. 
*/
/*
void getTopThreeCircularRentalStationsByMonth(stationsADT stations, int month, char ** topThree);
*/
#endif 