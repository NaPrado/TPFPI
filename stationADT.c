#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
// el tipo de dato es el mismo tanto para stationsNYC como stationsMON, lo que cambia es como obtenemos esos datos;

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
fd
struct CDT
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

struct q1{
    /* data */
};

struct q2{
    /* data */
};

struct q3{
    /* data */
};