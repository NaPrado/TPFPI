#ifndef __stationsADT_h_
#define __stationsADT_h_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define DAYS_IN_WEEK 7
#define MONTHS_IN_YEAR 12

typedef struct stationsCDT * stationsADT; 

/*
Se crea el ADT en donde se guardan los datos y se allocan todos los recursos que necesita.
*/
stationsADT newStationsGroup(int floorYear, int ceilingYear);
/*
Ordena segun un criterio util a la query 1 los recursos guardados en el ADT. Recibe al ADT que se quiera ordenar.
*/
void orderByCount(stationsADT stations);
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
Se debe llamar a esta funcion para poder usar las otras funciones Alpha
es un iniciador de iterador en orden alfabetico por nombres de estaciones
Si se lo llama 2 veces se reinicia
*/
void toBeginAlpha(stationsADT stations);
/*
Verifica que el primer elemento por orden alfabetico de nombres no sea nullo 
*/
int hasFirstAlpha(stationsADT stations);
/*
Consulta si existe un proximo elemento en orden alphabetico por nombres de estaciones
*/
int hasNextAlpha(stationsADT stations);
/*
avanza el iterador por orden alphabetico por nombres de estaciones
*/
void nextAlpha(stationsADT stations);
/* 
Verifica si la estacion iterada por nombre alfabetico tiene algun alquiler, 
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
int hasRentsAlpha(stationsADT stations);
/*
obtiene el nombre de la estacion iterada por nombre alfabetico,
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
char * getStationNameAlpha(stationsADT stations);
/*
obtiene el nombre del destino del viaje mas antiguo desde la estacion iterada por nombre alfabetico,
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
char * getOldestRentalStationNameEndAlpha(stationsADT stations);
/*
obtiene la fecha de partida del viaje mas antiguo desde la estacion iterada por nombre alfabetico,
para avanzar usar nextAlpha y no olvidar hasNextAlpha para chequear que exista
*/
struct tm getOldestRentalStartDateAlpha(stationsADT stations);

/* 
Se debe llamar a esta funcion para poder usar las otras funciones Count
es un iniciador de iterador Consulta si existe un proximo elemento por cantidad de viajes de totales
Si se lo llama 2 veces se reinicia
*/
void toBeginCount(stationsADT stations);
/*
Verifica que el primer elemento  por cantidad de viajes de totales no sea nullo 
*/
int hasFirstCount(stationsADT stations);
/*
Consulta si existe un proximo elemento por cantidad de viajes de totales
*/
int hasNextCount(stationsADT stations);
/*
avanza el iterador por cantidad de viajes de totales;
*/
void nextCount(stationsADT stations);

/*
Funciones count, las siguientes funciones se usan en conjunto con las tres de arriba
*/

/*
Obtiene la cantidad de alquileres hechos por miembros en la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
size_t getAmountRentalsByMembersCount(stationsADT stations);
/* 
Obtiene la cantidad de alquileres hechos por casuales en la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
size_t getAmountRentalsByCasualsCount(stationsADT stations);
/*
Obtiene la cantidad de alquileres hechos en la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
size_t getTotalAmountRentalsCount(stationsADT stations);
/*
Obtiene el nombre de la estacion iterada, 
para avanzar usar nextCount y no olvidar hasNextCount para chequear que exista
*/
char * getStationNameCount(stationsADT stations);


int hasRentsAlpha(stationsADT stations);

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

char * getMostPopularFromStationInAlphaOrder(stationsADT stations, size_t * amountOfTrips);

char ** getTopThreeCircularRentalStationsByMonth(stationsADT stations, int month);
#endif