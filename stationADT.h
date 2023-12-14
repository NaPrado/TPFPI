#ifndef __stationsADT_h_
#define __stationsADT_h_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DAYS_IN_WEEK 7
#define MONTHS_IN_YEAR 12

#define INDICATOR_HAS_NO_UPPER_LIMIT -1
#define EMPTY_IDENTIFIER "Empty"
#define INVALIDATE_INTERVAL -2

typedef struct stationsCDT * stationsADT; 

/*
Se crea el ADT en donde se guardan los datos y se allocan todos los recursos que necesita. Recibe un intervalo cerrado definido por los dos primeros
parametros para el funcionamiento de algunas funciones. De ser un intervalo invalido el flag valdra 0, caso contrario, 1. Para indicar que no se quiere
una cota superior usar INDICATOR_HAS_NO_UPPER_LIMIT. Si se quiere asegurar por alguna razon que el intervalo es invalido se puede usar la constate
INVALIDATE_INTERVAL en cualquiera de los dos parametros. El indicador validityFlag esta para que el usuario pueda saber si el intervalo enviado es valido.
*/
stationsADT newStationsGroup(int floorYear, int ceilingYear/* , int * validityFlag */);
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
Se debe llamar a esta funcion para poder usar las otras funciones de Q1
es un iniciador de iterador en el orden solicitado para Q1
Si se lo llama 2 veces se reinicia
*/
void toBeginQ1(stationsADT stations);
/*
Consulta si existe un proximo elemento para Q1
*/
int hasNextQ1(stationsADT stations);
/*
avanza el iterador de Q1
*/
void nextQ1(stationsADT stations);
/*
Obtiene la cantidad de alquileres hechos por miembros en la estacion iterada, 
para avanzar usar nextQ1 y no olvidar hasNextQ1 para chequear que exista
*/
size_t getMembersQ1(stationsADT stations);
/* 
Obtiene la cantidad de alquileres hechos por casuales en la estacion iterada, 
para avanzar usar nextQ1 y no olvidar hasNextQ1 para chequear que exista
*/
size_t getCasualsQ1(stationsADT stations);
/*
Obtiene la cantidad de alquileres hechos en la estacion iterada, 
para avanzar usar nextQ1 y no olvidar hasNextQ1 para chequear que exista
*/
size_t getTotalQ1(stationsADT stations);
/*
Obtiene el nombre de la estacion iterada, 
para avanzar usar nextQ1 y no olvidar hasNextQ1 para chequear que exista
*/
char * getStationNameQ1(stationsADT stations);
/* 
Se debe llamar a esta funcion para poder usar las otras funciones Q2
es un iniciador de iterador Consulta si existe un proximo elemento por Q2
Si se lo llama 2 veces se reinicia
*/
void toBeginQ2(stationsADT stations);
/*
Consulta si existe un proximo elemento por Q2
*/
int hasNextQ2(stationsADT stations);
/*
avanza el iterador Q2
*/
void nextQ2(stationsADT stations);
/* 
Verifica si la estacion iterada por Q2 tiene algun alquiler, 
para avanzar usar nextQ2 y no olvidar hasNextQ2 para chequear que exista
*/
int hasRentsQ2(stationsADT stations);
/*
obtiene el nombre de la estacion iterada por Q2,
para avanzar usar nextQ2 y no olvidar hasNextQ2 para chequear que exista
*/
char * getStationNameQ2(stationsADT stations);
/*
obtiene el nombre del destino del viaje mas antiguo desde la estacion iterada por Q2,
para avanzar usar nextQ2 y no olvidar hasNextQ2 para chequear que exista
*/
char * getOldestRentalStationNameEndQ2(stationsADT stations);
/*
obtiene la fecha de partida del viaje mas antiguo desde la estacion iterada por Q2,
para avanzar usar nextQ2 y no olvidar hasNextQ2 para chequear que exista
*/
struct tm getOldestRentalStartDateQ2(stationsADT stations);


/*
Retorna un vector de dim DAYS_IN_WEEK con la cantidad de viajes empezados por dia de la semana
siendo v[0]= a la cantidad de los lunes, v[1]= a la de los martes, etc..
*/
size_t * getStartedTripsQ3(stationsADT stations);
/*
Retorna un vector de dim DAYS_IN_WEEK con la cantidad de viajes finalizados por dia de la semana
siendo v[0]= a la cantidad de los lunes, v[1]= a la de los martes, etc..
*/
size_t * getEndedTripsQ3(stationsADT stations);
/*
Se debe llamar a esta funcion para poder usar las otras funciones Q4
es un iniciador de iterador Consulta si existe un proximo elemento por Q4
Si se lo llama 2 veces se reinicia
*/
void toBeginQ4(stationsADT stations);
/*
Consulta si existe un proximo elemento por Q4
*/
int hasNextQ4(stationsADT stations);
/*
avanza el iterador Q4
*/
void nextQ4(stationsADT stations);
/*
Obtiene el nombre de la estacion iterada, 
para avanzar usar nextQ4 y no olvidar hasNextQ4 para chequear que exista
*/
char * getStationNameQ4(stationsADT stations);
/*
Devuelve un string con el nombre de la estacion mas popular para aquella en la que el iterador alfabetico esta posicionado. Es responsabilidad del usuario
que el iterador este inicializado y avanzar por el mismo. Ademas recibe por parametro un puntero a la cantidad de alquileres que hayan terminado en la
estacion mas popular
*/
char * getMostPopularFromStationQ4(stationsADT stations, size_t * amountOfTrips);






/*
La funcion recibe el mes en el que se le quiere averiguar cual es el top tres estaciones por cantidad de viajes circulares. Recibe como parametro 
un vector de dimension tres. 
*/
/*
void getTopThreeCircularRentalStationsByMonth(stationsADT stations, int month, char ** topThree);
*/
#endif 