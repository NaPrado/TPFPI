#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "stationADT.h"
// el tipo de dato es el mismo tanto para stationsNYC como stationsMON, lo que cambia es como obtenemos esos datos;

/* 
    Info importante de cada archivo
    Para bykes:
        start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member
        2021-09-20 06:31:28;348;2021-09-20 07:02:22;332;1
    Para stations:
        pk;name;latitude;longitude
        327;Sanguinet / de Maisonneuve;45.513405;-73.562594
*/

// ---------------------------------IDEA-----------------------------------------
// proponemos que el cdt contenga dos cosas primero una lista de estaciones
// segundo un arbol de ids
// esto con el proposito de que primero se cargan las informaciones de las estaciones existentes del archivo stations
// a su vez se iran cargando en paralelo las ids existentes de cada estacion a un arbol binario de busqueda 
// segundo se va copiar los viajes en el nodo de cada estacion si las estaciones de salida y llegada son validas (osea estan en el arbol)

typedef struct rental * pRental ;
struct rental //datos del archivo Bike se guardaran en formato de lista ordenada por stationIdStart 
{
    // size_t stationIdStart; redundante con la lista de estaciones
    struct tm dateStart;
    size_t stationIdStartEnd;
    struct tm dateStartEnd;
    bool is_member;
    pRental tail;
};

struct station
{
    char * stationName;
    // long double latitude; ver si es necesario
    // long double longitude;
    // size_t id;
    pRental oldestRental;
    size_t amountRentalsByMembers;
    size_t amountRentalsByCasuals;
    size_t totalAmountRentals;
    struct station * tail;
};

struct stationCDT
{
    struct station * stations; //lista estaciones orden alfabetico (osea puntero a primer nodo)
};


//esta funcion debera se llamada para asignar los tiempos de entrada/salida en las structs correspondientes
struct tm * assignTime(/*la funcion deberia recibir tiempo de alguna forma, no se si como char * con todos los datos y luego fragmentarlos, como varios char * en partes o como varios enteros*/)
{
    struct tm * date;
    date->tm_hour/*  = hora */;
    //el resto de los campos asignados
    return date;
}

struct stationsIdBST{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    struct station * associatedStation;
    struct stationsIdBST * left;
    struct stationsIdBST * right;
};


char isValidRental(){

}

stationADT inicializer(){

}

stationADT newStation(void){

}

stationADT addStation(){

}

//quiza no haga falta 
stationADT deleteStation(){

}

void freeStation(){

}