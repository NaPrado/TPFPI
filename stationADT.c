#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
// el tipo de dato es el mismo tanto para stationsNYC como stationsMON, lo que cambia es como obtenemos esos datos;

/* info importante de cada archivo
    Para bykes:
        start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member
        2021-09-20 06:31:28;348;2021-09-20 07:02:22;332;1
    Para stations:
        pk;name;latitude;longitude
        327;Sanguinet / de Maisonneuve;45.513405;-73.562594
*/

typedef struct stationCDT * stationADT; 

typedef struct rental * pRental ;
struct rental //datos del archivo Bike se guardaran en formato de lista ordenada por stationIdStart 
{
    __u_short stationIdStart;
    struct tm dateStart;
    __u_short stationIdStartEnd;
    struct tm dateStartEnd;
    bool is_member;
    pRental tail;
};

struct station
{
    char * stationName;
    long latitude;
    long longitude;
    __u_short id;
};

struct stationCDT
{
    struct station * stations;
    struct rental * rents;
};


//esta funcion debera se llamada para asignar los tiempos de entrada/salida en las structs correspondientes
struct tm * assignTime(/*la funcion deberia recibir tiempo de alguna forma, no se si como char * con todos los datos y luego fragmentarlos, como varios char * en partes o como varios enteros*/)
{
    struct tm * date;
    date->tm_hour/*  = hora */;
    //el resto de los campos asignados
    return date;
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