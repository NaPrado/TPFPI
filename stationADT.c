#include <stdio.h>
#include <stdbool.h>
// el tipo de dato es el mismo tanto para stationsNYC como stationsMON, lo que cambia es como obtenemos esos datos;
//hsxvnjsvjksjvksvklnlksndlk
typedef struct dateHour
{
    __u_short year;
    __u_char month;
    __u_char day;
    __u_char hour;
    __u_char min;
    __u_char sec;
}TDateHour;

struct bike
{
    TDateHour start_date;
    __u_short start_station_id;
    TDateHour end_date;
    __u_short end_station_id;
    bool is_member;
    bool rideable_type; //solo para NYC
};

struct stationADT
{
    /* Habria que sumar una lista o un arreglo de struct bike, teneiendo en cuenta que bikes se van a 
    modificar de una estacion a otra y tenerlas ordenadas por id es favorable, y que 
    no sabemos cuantas hay creo que lo conveniente es poner una lista */
    char * stationName;
    long latitude;
    long longitude;
    __u_short id;
    __u_char bikesAvailable;
};

