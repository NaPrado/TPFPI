#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
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





static pRental checkIfCircular(pRental rent, char * currentStationName){
    if (strcmp(currentStationName,rent->stationNameEnd)==0){
        return rent;
    }
    checkIfCircular(rent->tail,currentStationName);
}

static char * dateToStr(struct tm * date){
    char * day = intToStr(date->tm_mday);
    char * month = intToStr(date->tm_mon);
    char * year = intToStr(date->tm_hour);
    char * mins = intToStr(date->tm_min);
}



static void writeQ2Rec(pStation stations, htmlTable tablaQ2, FILE * csvQ2,char col){
    if (stations==NULL)
        return;
    pRental rent = checkIfCircular(stations->oldestRental,stations->stationName);
    //hay que pasar los ints a str
    //"DD/MM/YYYY HH:mm"
    char * col1=strcpy(col1,strcat(stations->stationName,col)); 
    char * col2=strcpy(col2,strcat(rent->stationNameEnd,col));
    char * col3=strcpy(col3,strcat(dateToStr(rent->dateEnd),'\n'));
    addHTMLRow(tablaQ2,col1,col2,col3);
    char * completeLine=strcpy(strcpy(col1,col2),col3);
    fputs(completeLine,csvQ2);
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
