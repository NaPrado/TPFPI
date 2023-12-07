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
    pRental oldestRental; //lista
    size_t amountRentalsByMembers;  //contadores para q1
    size_t amountRentalsByCasuals;  //contadores para q1
    size_t totalAmountRentals;      //contadores para q1
    pStation tailAlpha;
    pStation tailCount;             //util para q1
};

struct stationCDT
{
    pStation firstAlpha;
    pStation firstCount;
};                              

static pStation link(pStation station,pStation listCount){
    if(listCount == NULL || station->totalAmountRentals <= listCount->totalAmountRentals){
        station->tailCount = listCount;
        listCount->tailCount = station;
        return listCount;
    }
    listCount->tailCount = link(station,listCount->tailCount);
    return listCount;
}

static void orderByCount(stationADT stations){
    pStation aux = stations->firstAlpha;
    while (aux != NULL){
        stations->firstCount = link(aux,stations->firstCount);
        aux = aux->tailAlpha;
    }
    return;
}

static void writeQ1Rec(pStation stations, htmlTable tablaQ1, FILE * csvQ1){
    if (stations==NULL)
        return;
    addHTMLRow(tablaQ1,stations->stationName,stations->amountRentalsByMembers,stations->amountRentalsByCasuals,stations->totalAmountRentals);
    fprintf(csvQ1,"%s;%d;%d;%d\n",stations->stationName,stations->amountRentalsByMembers,stations->amountRentalsByCasuals,stations->totalAmountRentals);
    writeQ1Rec(stations->tailCount,tablaQ1,csvQ1);
    return;
}

static void writeQ1(stationADT stations){
    errno = 0;
    FILE * csvQ1 = fopen("query1.csv","wt");
    if(errno != 0 || csvQ1==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fprintf(csvQ1,"bikeStation;memberTrips;casualTrips;allTrips\n");
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    writeQ1Rec(stations->firstCount,tablaQ1,csvQ1);//funcion recursva o iterativa que carga tanto html como csv
    closeHTMLTable(tablaQ1);
    fclose(csvQ1);
}


void query1(stationADT stations){
    orderByCount(stations);
    writeQ1(stations);//carga tanto html como csv
};


