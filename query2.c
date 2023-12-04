#include <string.h>
#include <errno.h>
#include <stdlib.h>
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





pRental checkIfCircular(pRental rent, char * currentStationName){
    if (strcmp(currentStationName,rent->stationNameEnd)==0){
        return rent;
    }
    checkIfCircular(rent->tail,currentStationName);
}


static void writeQ2Rec(pStation stations, htmlTable tablaQ2, FILE * csvQ2,char col){
    if (stations==NULL)
        return;
    pRental rent = checkIfCircular(stations->oldestRental,stations->stationName);
    //hay que pasar los ints a str
    addHTMLRow(tablaQ2,strcat(stations->stationName,col),strcat(rent->stationNameEnd,col),strcat(rent->dateStart,'\n'));
    fputs(strcat(stations->stationName,col),csvQ2);
    fputs(strcat(rent->stationNameEnd,col),csvQ2);
    fputs(strcat(rent->dateStart,'\n'),csvQ2);
    writeQ2Rec(stations->tailAlpha,tablaQ2,csvQ2,col);
    return;
}
//DD/MM/YYYY HH:mm
static void writeQ2(struct stationCDT * stations,char countryFormat /* c || n */ ){
    errno = 0;
    FILE * csvQ2 = fopen("query2.csv","wt");
    if(errno != 0 || csvQ2==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    char col;//caracter que separa las columnas
    col=(countryFormat=='c') ? '/':';';
    
    fputs("bikeStation;bikeEndStation;oldestDateTime\n",csvQ2);
    htmlTable tablaQ2 = newTable("query2.html",3,"bikeStation","bikeEndStation","oldestDateTime");
    writeQ2Rec(stations->firstAlpha,tablaQ2,csvQ2,col);//funcion recursva o iterativa que carga tanto html como csv
    closeHTMLTable(tablaQ2);
    fclose(csvQ2);
}


q2(struct stationCDT * stations,char countryFormat){
    writeQ2(stations,countryFormat);//carga tanto html como csv
};
