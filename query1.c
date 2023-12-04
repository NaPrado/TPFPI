#include "stationADT.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "stationADT.h"
#include "libreria HTML/htmlTable.h"
typedef struct rental * pRental ;
struct rental //datos del archivo Bike se guardaran en formato de lista ordenada por stationIdStart 
{
    // size_t stationIdStart; redundante con la lista de estaciones
    struct tm * dateStart;
    char * stationNameEnd;
    struct tm * dateEnd;
    //bool is_member;
    pRental tail;
};

typedef struct station * pStation ;

struct station //lista
{
    char * stationName;
    // long double latitude; ver si es necesario
    // long double longitude;
    // size_t id;
    pRental oldestRental; //lista
    size_t amountRentalsByMembers;  //contadores para q1
    size_t amountRentalsByCasuals;  //contadores para q1
    size_t totalAmountRentals;      //contadores para q1
    pStation tailAlpha;
    pStation tailCount;             //util para q1
};

struct stationCDT
{
    pStation firstAlpha;//lista estaciones orden alfabetico (osea digamos puntero a primer nodo)
    pStation firstCount;//lista oredenada segun cantidad de viajes iniciados en esa estacion
};                              //osea digamos soy nahue, es necesario esto ??????????????????????????????????????????????????????????????????????????????????????????

static void writeQ1Rec(pStation stations, htmlTable tablaQ1, FILE * csvQ1){
    if (stations==NULL)
        return;
    
    //hay que pasar los ints a str
    addHTMLRow(tablaQ1,stations->stationName,stations->amountRentalsByMembers,stations->amountRentalsByCasuals,stations->totalAmountRentals);
    fputs(strcat(stations->stationName,";"),csvQ1);
    fputs(strcat(stations->amountRentalsByMembers,";"),csvQ1);
    fputs(strcat(stations->amountRentalsByCasuals,";"),csvQ1);
    fputs(strcat(stations->totalAmountRentals,"\n"),csvQ1);
    writeQ1Rec(stations->tailCount,tablaQ1,csvQ1);
    return;
}

static void writeQ1(struct stationCDT * stations){
    errno = 0;
    FILE * csvQ1 = fopen("query1.csv","wt");
    if(errno != 0 || csvQ1==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fputs("bikeStation;memberTrips;casualTrips;allTrips\n",csvQ1);
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    writeQ1Rec(stations->firstCount,tablaQ1,csvQ1);//funcion recursva o iterativa que carga tanto html como csv
    closeHTMLTable(tablaQ1);
    fclose(csvQ1);
}


q1(struct stationCDT * stations){
    ordenarPorCantidadDeViajes(stations);//no implementada
    writeQ1(stations);//carga tanto html como csv
};

