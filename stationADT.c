#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include "stationADT.h"
#include "treeADT.h"


#define MEMBER 1
#define CASUAL 0 

enum DAYS           
{     
    monday=0,       
    tuesday,
    wednesday,     
    thursday,
    friday,
    saturday,     
    sunday,
} workday;


struct nameIdAndCounter{
    char * name;
    size_t id;
    size_t counter;
};

typedef struct nameIdAndCounter * pNameIdAndCounter;

typedef struct rental * pRental ;
struct rental 
{
    char * stationNameEnd;
    struct tm dateStart;
    struct tm dateEnd;
};

typedef struct station * pStation ;
struct station 
{
    char * stationName;
    pNameIdAndCounter * mostPopularEndStations;
    size_t sizeOfMostPopular;
    pRental oldestRental;
    size_t amountRentalsByMembers;
    size_t amountRentalsByCasuals;
    size_t totalAmountRentals;
    pStation tailAlpha;
    pStation tailCount;
};

struct topThreeCircularStations{
    pNameIdAndCounter * topOfMonth;
    size_t sizeOfTopOfMonth;
};

struct stationsCDT
{
    size_t startedTrips[DAYS_IN_WEEK];
    size_t endedTrips[DAYS_IN_WEEK];
    struct topThreeCircularStations topThreeInMonth[MONTHS_IN_YEAR];
    pStation firstAlpha;
    pStation iterAlpha;
    pStation firstCount;
    pStation iterCount;
    bstADT tree;
};                              


static char* copyString(const char * origin) {
    // Obtener la longitud de la cadena de origen
    size_t length = strlen(origin);

    // Asignar memoria dinámica para la cadena de destino
    char *toReturn = (char*)malloc((length + 1) * sizeof(char));

    // Verificar si la asignación de memoria fue exitosa
    if (toReturn == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    // Copiar la cadena de origen a la zona de memoria dinámica
    strcpy(toReturn, origin);

    // Devolver la dirección de la zona de memoria asignada
    return toReturn;
}


stationsADT newStationsGroup(void){
    stationsADT new = calloc(1,sizeof(struct stationsCDT));
    new->tree=newtree();
    return new;
}

static pStation addStationRec(pStation alphaList,char * stationName, bstADT tree, size_t stationId){
    int c;
    if(alphaList == NULL ||  (c=strcasecmp(alphaList->stationName, stationName)) > 0){
        //incorporacion a la lista
        pStation newNode = calloc(1,sizeof(struct station));
        newNode->stationName = stationName;
        newNode->tailAlpha =alphaList;
        //incorporacion a el BST
        addToTree(tree,stationId,newNode);
        return newNode;
    }
    if(c<0){
        alphaList->tailAlpha=addStationRec(alphaList->tailAlpha,stationName,tree,stationId);
    }
    return alphaList;
}


void addStation(stationsADT stations,char * stationName, size_t stationId){
    char * name =copyString(stationName);
    stations->firstAlpha=addStationRec(stations->firstAlpha,name,stations->tree,stationId);
    return;
}

static int getWeekDay(int day,int month,int year){
    int weekday  = (day += month < 3 ? year-- : year - 2, 23*month/9 + day + 4 + year/4- year/100 + year/400)%7;
    if (weekday==0){
        return sunday;
    }
    else{
        return weekday-1;
    }
}

static void countTrips(stationsADT stations,struct tm startDate,struct tm endDate){
    stations->startedTrips[getWeekDay(startDate.tm_mday,startDate.tm_mon+1,startDate.tm_year+1900)]++;
    stations->endedTrips[getWeekDay(endDate.tm_mday,endDate.tm_mon+1,endDate.tm_year+1900)]++;
    return;
}

void addRental(struct tm startDate,size_t startId,struct tm endDate, size_t endId, char association, stationsADT stations){
    pStation startStation,endStation;
    if ((startStation = existId(startId,stations->tree))==NULL||(endStation = existId(endId,stations->tree))==NULL){
        return;
    }
    
    countTrips(stations,startDate,endDate);
    startStation->totalAmountRentals += 1;
    if(association == MEMBER){
        startStation->amountRentalsByMembers += 1;
    }
    if(association == CASUAL){
        startStation->amountRentalsByCasuals += 1;
    }

    if (startId==endId){
        return;
    }
    else if (startStation->oldestRental==NULL || difftime(mktime(&startDate),mktime(&(startStation->oldestRental->dateStart))) < 0 ){
            pRental newRental=calloc(1,sizeof(struct rental));
            newRental->dateEnd=endDate;
            newRental->dateStart=startDate;
            newRental->stationNameEnd=endStation->stationName;
            free(startStation->oldestRental);
            startStation->oldestRental=newRental;
    }
}

static void freeRentals(pStation stations){
    if (stations==NULL||stations->oldestRental==NULL){
        return;
    }
        free(stations->oldestRental);
}

static void freeStations(pStation stationList){
    if(stationList == NULL){
        return;
    }
    freeRentals(stationList);
    freeStations(stationList->tailAlpha);
    free(stationList->stationName);
    free(stationList);
    return;
}

void freeAssets(stationsADT stations){
    freeStations(stations->firstAlpha);
    free(stations);
}


//NO ENTIENDO ESTA FUNCION, PORQUE NO UN FIRSTCOUNT
// Función para obtener el puntero al nodo con mayor cantidad de viajes
/* static pStation getNodeWithMaxRentals(pStation stations) {
    pStation maxNode = NULL;
    while (stations != NULL) {
        if (maxNode == NULL || stations->totalAmountRentals > maxNode->totalAmountRentals) {
            maxNode = stations;
        }
        stations = stations->tailAlpha;
    }
    return maxNode;
} */

static pStation link(pStation listAlpha,pStation listCount){
    int c;
    if(listCount == NULL || (c = listAlpha->totalAmountRentals - listCount->totalAmountRentals) > 0){
        listAlpha->tailCount = listCount;
        return listAlpha;
    }
    if(c<=0)
        listCount->tailCount = link(listAlpha,listCount->tailCount);
    return listCount;
}

void orderByCount(stationsADT stations){
    pStation aux = stations->firstAlpha;
    while (aux != NULL){
        stations->firstCount = link(aux,stations->firstCount);
        aux = aux->tailAlpha;
    }
    
    return;
}

void toBeginAlpha(stationsADT stations){
    stations->iterAlpha=stations->firstAlpha;
    return;
}

int hasFirstAlpha(stationsADT stations){
    return stations->firstAlpha!=NULL;
}

int hasNextAlpha(stationsADT stations){
    return (stations->iterAlpha->tailAlpha!=NULL);
}

void nextAlpha(stationsADT stations){
  if ( !hasNextAlpha(stations)) {
    fprintf(stderr, "Uso invalido de iterador\n");
    exit(1);
  }
  stations->iterAlpha=stations->iterAlpha->tailAlpha;
  return;
}

int hasRentsAlpha(stationsADT stations){
    if ( !hasNextAlpha(stations)) {
    fprintf(stderr, "Uso invalido de iterador\n");
    exit(1);
    }
    return stations->iterAlpha->oldestRental!=NULL;//tambien se puede preguntar si total es 0
}


char * getStationNameAlpha(stationsADT stations){
    if ( !hasNextAlpha(stations)) {
    fprintf(stderr, "Uso invalido de iterador\n");
    exit(1);
    }
    return stations->iterAlpha->stationName;
}

char * getOldestRentalStationNameEndAlpha(stationsADT stations){
    if ( !hasNextAlpha(stations)) {
    fprintf(stderr, "Uso invalido de iterador\n");
    exit(1);
    }
    return stations->iterAlpha->oldestRental->stationNameEnd;
}

struct tm getOldestRentalStartDateAlpha(stationsADT stations){
    if ( !hasNextAlpha(stations)) {
    fprintf(stderr, "Uso invalido de iterador\n");
    exit(1);
    }
    return stations->iterAlpha->oldestRental->dateStart;   
}





void toBeginCount(stationsADT stations){
    stations->iterCount=stations->firstCount;
    return;
}


int hasFirstCount(stationsADT stations){
    return stations->firstCount!=NULL;
}


int hasNextCount(stationsADT stations){
    return (stations->iterCount->tailCount!=NULL);
}

void nextCount(stationsADT stations) {
  if ( !hasNextCount(stations)) {
    fprintf(stderr, "Uso invalido de iterador\n");
    exit(1);
  }
  stations->iterCount=stations->iterCount->tailCount;
  return;
}


size_t getAmountRentalsByMembersCount(stationsADT stations){
    if (stations->iterCount==NULL)
    {
        fprintf(stderr, "No se inicio el iterador\n");
        exit(1);
    }
    return stations->firstCount->amountRentalsByMembers;
}

size_t getAmountRentalsByCasualsCount(stationsADT stations){
    if (stations->iterCount==NULL)
    {
        fprintf(stderr, "No se inicio el iterador\n");
        exit(1);
    }
    return stations->firstCount->amountRentalsByCasuals;
}

size_t getTotalAmountRentalsCount(stationsADT stations){
    if (stations->iterCount==NULL)
    {
        fprintf(stderr, "No se inicio el iterador\n");
        exit(1);
    }
    return stations->firstCount->totalAmountRentals;
}

char * getStationNameCount(stationsADT stations){
    if (stations->iterCount==NULL)
    {
        fprintf(stderr, "No se inicio el iterador\n");
        exit(1);
    }
    return stations->iterCount->stationName;
}



size_t * getStartedTrips(stationsADT stations){
    return stations->startedTrips;
}

size_t * getEndedTrips(stationsADT stations){
    return stations->endedTrips;
}