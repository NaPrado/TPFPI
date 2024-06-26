#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include "stationADT.h"
#include "treeADT.h"


#define MEMBER 1
#define CASUAL 0 
#define NO_LIMITS 3
#define NO_UPPER_LIMIT 4
#define ALL_LIMITS 5
#define WEEK_ALIGMENT 1
#define MONTH_ALIGMENT 1
#define YEAR_ALIGMENT 1900 
#define YEAR_FLOOR_ARGUMENT 3
#define YEAR_CEIL_ARGUMENT 4

#define ERROR_ALLOCATION "Error alocando memoria\n"
#define ARG_ERROR "Error en pasaje de argumentos para intervalo de años\n"
#define WARNING_ARGS_MESSAGE "Advetencia, la cantidad de argumentos es mayor a la solicitada\n"
#define ERROR_ITER "Uso invalido de iterador\n"
#define ITER_NOT_ASSIGN_ERROR "No se ha inicializado el iterador\n"

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
    struct nameIdAndCounter * mostPopularEndStations;
    size_t sizeOfMostPopular;
    pRental oldestRental;
    size_t amountRentalsByMembers;
    size_t amountRentalsByCasuals;
    size_t totalAmountRentals;
    pStation tailAlpha;
    pStation tailCount;
};


struct stationsCDT
{
    size_t startedTrips[DAYS_IN_WEEK];
    size_t endedTrips[DAYS_IN_WEEK];
    pStation firstAlpha;
    pStation firstCount;
    pStation iterAlpha;
    pStation iterCount;
    bstADT tree;
    int floorYear;
    int ceilingYear; //de ser igual a INDICATOR_HAS_NO_UPPER_LIMIT significa q no hay limite superior
};

#define IS_WITHIN_INTERVAL(x,y,z) (((z) == INDICATOR_HAS_NO_UPPER_LIMIT) && ((y)==INDICATOR_HAS_NO_LOWER_LIMIT))?1:((z) == INDICATOR_HAS_NO_UPPER_LIMIT)? ((x)>=(y)):(((x)>=(y)) && (x)<=(z))

static char* copyString(const char * origin) {
    // Obtener la longitud de la cadena de origen
    size_t length = strlen(origin);

    // Asignar memoria dinámica para la cadena de destino
    char *toReturn = (char*)malloc((length + 1) * sizeof(char));

    // Verificar si la asignación de memoria fue exitosa
    if (toReturn == NULL) {
        perror(ERROR_ALLOCATION);
        exit(EXIT_FAILURE);
    }

    // Copiar la cadena de origen a la zona de memoria dinámica
    strcpy(toReturn, origin);

    // Devolver la dirección de la zona de memoria asignada
    return toReturn;
}

static int isValidInterval(int floor, int ceil){
    return (floor >= 0 && ceil >= 0 && floor <= ceil);
}

static int strIsNumber(const char *string){
    int i=0;
    while (*(string+i)){
        if (!isdigit(*(string+i))){
            return 0;
        }
        i++;
    }
    return 1;
}

static int strToInt(const char *string, int *num) {
    if(strIsNumber(string)){
        *num=atoi(string);
        return 1;
    }
    else{
        return 0;
    }
}

static void yearValidator(int argc, const char * argv[], int * floorYear, int * ceilYear){
    if (argc == NO_LIMITS){ //no me pasan años
        *floorYear=INDICATOR_HAS_NO_LOWER_LIMIT;
        *ceilYear=INDICATOR_HAS_NO_UPPER_LIMIT;
        return;
    }
    else if(argc == NO_UPPER_LIMIT){ //se recibe un año entonces asumo q es el piso
        if(strToInt(argv[YEAR_FLOOR_ARGUMENT],floorYear) && *floorYear>=0){
            *ceilYear = INDICATOR_HAS_NO_UPPER_LIMIT;
            return;
        }
        else{
            printf(ARG_ERROR);
            exit(EXIT_FAILURE);
        }
    }
    else if(argc == ALL_LIMITS){ //se reciben ambos años
        if(strToInt(argv[YEAR_FLOOR_ARGUMENT],floorYear) && strToInt(argv[YEAR_CEIL_ARGUMENT],ceilYear) && isValidInterval(*floorYear,*ceilYear)){
            return;
        }
        else{
            printf(ARG_ERROR);
            exit(EXIT_FAILURE);
        }
    }
    else if (argc > ALL_LIMITS && WARNING_FLAG){
        printf(WARNING_ARGS_MESSAGE);
        return;
    }
    else{
        printf(ARG_ERROR);
        exit(EXIT_FAILURE);
    }
}

stationsADT newStationsGroup(int argc, const char * argv[]){
    int ceilingYear,floorYear;
    yearValidator(argc,argv,&floorYear,&ceilingYear);
    stationsADT new = calloc(1,sizeof(struct stationsCDT));
    if (new == NULL) {
        perror(ERROR_ALLOCATION);
        exit(EXIT_FAILURE);
    }
    new->tree=newtree();
    new->ceilingYear = ceilingYear;
    new->floorYear = floorYear;
    return new;
}

static pStation addStationRec(pStation alphaList,char * stationName, bstADT tree, size_t stationId){
    int c;
    if(alphaList == NULL ||  (c=strcasecmp(alphaList->stationName, stationName)) > 0){
        //incorporacion a la lista
        pStation newNode = calloc(1,sizeof(struct station));
        if(newNode == NULL){
            printf(ERROR_ALLOCATION);
            exit(EXIT_FAILURE);
        }
        newNode->stationName = copyString(stationName);
        newNode->tailAlpha = alphaList;
        //incorporacion a el BST
        addToTree(tree,stationId,newNode);
        return newNode;
    }
    if(c<0){
        alphaList->tailAlpha = addStationRec(alphaList->tailAlpha,stationName,tree,stationId);
    }
    return alphaList;
}

void addStation(stationsADT stations,char * stationName, size_t stationId){
    stations->firstAlpha = addStationRec(stations->firstAlpha,stationName,stations->tree,stationId);
    return;
}

static int getWeekDay(int day,int month,int year){
    int weekday  = (day += month < 3 ? year-- : year - 2, 23*month/9 + day + 4 + year/4- year/100 + year/400)%7;
    if (weekday==0){
        return sunday;
    }
    else{
        return weekday-WEEK_ALIGMENT;
    }
}

static void countTrips(stationsADT stations,struct tm startDate,struct tm endDate){
    stations->startedTrips[getWeekDay(startDate.tm_mday,startDate.tm_mon+MONTH_ALIGMENT,startDate.tm_year+YEAR_ALIGMENT)]++;
    stations->endedTrips[getWeekDay(endDate.tm_mday,endDate.tm_mon+MONTH_ALIGMENT,endDate.tm_year+YEAR_ALIGMENT)]++;
    return;
}

static int isWithinYearInterval(int startYear, int endYear, stationsADT stations){
    return (IS_WITHIN_INTERVAL(startYear,stations->floorYear,stations->ceilingYear) && IS_WITHIN_INTERVAL(endYear,stations->floorYear,stations->ceilingYear));
}

static void addToMostPopular(pStation startStation, size_t endId, char * endStationName){
    startStation->mostPopularEndStations = realloc(startStation->mostPopularEndStations, (startStation->sizeOfMostPopular + 1) * sizeof(struct nameIdAndCounter));
    if (startStation->mostPopularEndStations == NULL) {
        perror(ERROR_ALLOCATION);
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i<startStation->sizeOfMostPopular; i++){
        if(startStation->mostPopularEndStations[i].id == endId){
            startStation->mostPopularEndStations[i].counter ++;
            return;
        }
    }
    startStation->mostPopularEndStations[startStation->sizeOfMostPopular].counter = 1;
    startStation->mostPopularEndStations[startStation->sizeOfMostPopular].name = endStationName;
    startStation->mostPopularEndStations[startStation->sizeOfMostPopular].id = endId;
    startStation->sizeOfMostPopular++;
}

void addRental(struct tm startDate,size_t startId,struct tm endDate, size_t endId, char association, stationsADT stations){
    pStation startStation,endStation;
    if ((startStation = existId(startId,stations->tree)) == NULL||(endStation = existId(endId,stations->tree)) == NULL){
        return;
    }
    
    countTrips(stations,startDate,endDate);
    startStation->totalAmountRentals ++;
    if(association == MEMBER){
        startStation->amountRentalsByMembers ++;
    }
    if(association == CASUAL){
        startStation->amountRentalsByCasuals ++;
    }
    if (startId != endId){
        if (startStation->oldestRental == NULL || difftime(mktime(&startDate),mktime(&(startStation->oldestRental->dateStart))) < 0 ){
            pRental newRental=calloc(1,sizeof(struct rental));
            if(newRental == NULL){
                printf(ERROR_ALLOCATION);
                exit(EXIT_FAILURE); 
            }
            newRental->dateEnd=endDate;
            newRental->dateStart=startDate;
            newRental->stationNameEnd=endStation->stationName;
            free(startStation->oldestRental);
            startStation->oldestRental=newRental;
        }
        if(isWithinYearInterval(startDate.tm_year+YEAR_ALIGMENT,endDate.tm_year+YEAR_ALIGMENT, stations)){
            addToMostPopular(startStation, endId, endStation->stationName);
        } 
    }
}

static void freeRentals(pStation stations){
    if (stations == NULL || stations->oldestRental == NULL){
        return;
    }
    free(stations->oldestRental);
}

static void freeStations(pStation stationList){
    if(stationList == NULL){
        return;
    }
    free(stationList->stationName);
    freeRentals(stationList);
    freeStations(stationList->tailAlpha);
    if(stationList->mostPopularEndStations != NULL){
        free(stationList->mostPopularEndStations);
    }
    free(stationList);
    return;
}

void freeAssets(stationsADT stations){
    freeTree(stations->tree);
    freeStations(stations->firstAlpha);
    free(stations);
}

// Función para obtener el puntero al nodo con mayor cantidad de viajes
static pStation getNodeWithMaxRentals(pStation stations) {
    pStation maxNode = NULL;
    while (stations != NULL) {
        if (maxNode == NULL || stations->totalAmountRentals > maxNode->totalAmountRentals) {
            maxNode = stations;
        }
        stations = stations->tailAlpha;
    }
    return maxNode;
}

static pStation linkCount(pStation listAlpha,pStation listCount){
    int c;
    if(listCount == NULL || (c = listAlpha->totalAmountRentals - listCount->totalAmountRentals) > 0){
        listAlpha->tailCount = listCount;
        return listAlpha;
    }
    if(c<=0){
        listCount->tailCount = linkCount(listAlpha,listCount->tailCount);
    }
    return listCount;
}

static void orderByCount(stationsADT stations){
    pStation aux = stations->firstAlpha;
    while (aux != NULL){
        stations->firstCount = linkCount(aux,stations->firstCount);
        aux = aux->tailAlpha;
    }
    return;
}

void toBeginCount(stationsADT stations){
    if (stations->iterCount==NULL)
    {
        orderByCount(stations);
    }
    stations->iterCount=getNodeWithMaxRentals(stations->firstAlpha);
    return;
}

int hasNextCount(stationsADT stations){
    return (stations->iterCount!=NULL);
}

void nextCount(stationsADT stations) {
  if (!hasNextCount(stations)) {
    fprintf(stderr, ERROR_ITER);
    exit(EXIT_FAILURE);
  }
  stations->iterCount=stations->iterCount->tailCount;
  return;
}

size_t getMembersCount(stationsADT stations){
    if (stations->iterCount == NULL){
        fprintf(stderr, ITER_NOT_ASSIGN_ERROR);
        exit(EXIT_FAILURE);
    }
    return stations->iterCount->amountRentalsByMembers;
}

size_t getCasualsCount(stationsADT stations){
    if (stations->iterCount == NULL){
        fprintf(stderr, ITER_NOT_ASSIGN_ERROR);
        exit(EXIT_FAILURE);
    }
    return stations->iterCount->amountRentalsByCasuals;
}

size_t getTotalCount(stationsADT stations){
    if (stations->iterCount == NULL){
        fprintf(stderr, ITER_NOT_ASSIGN_ERROR);
        exit(EXIT_FAILURE);
    }
    return stations->iterCount->totalAmountRentals;
}

char * getStationNameCount(stationsADT stations){
    if (stations->iterCount == NULL){
        fprintf(stderr, ITER_NOT_ASSIGN_ERROR);
        exit(EXIT_FAILURE);
    }
    return stations->iterCount->stationName;
}

int hasRentsAlpha(stationsADT stations){
    if (!hasNextAlpha(stations)){
    fprintf(stderr, ERROR_ITER);
    exit(EXIT_FAILURE);
    }
    return stations->iterAlpha->oldestRental!=NULL;
}

char * getOldestRentalStationNameEndAlpha(stationsADT stations){
    if (!hasNextAlpha(stations)) {
    fprintf(stderr, ERROR_ITER);
    exit(EXIT_FAILURE);
    }
    return stations->iterAlpha->oldestRental->stationNameEnd;
}

struct tm getOldestRentalStartDateAlpha(stationsADT stations){
    if (!hasNextAlpha(stations)) {
    fprintf(stderr, ERROR_ITER);
    exit(EXIT_FAILURE);
    }
    return stations->iterAlpha->oldestRental->dateStart;   
}

size_t * getStartedTrips(stationsADT stations){
    return stations->startedTrips;
}

size_t * getEndedTrips(stationsADT stations){
    return stations->endedTrips;
}

void toBeginAlpha(stationsADT stations){
    stations->iterAlpha=stations->firstAlpha;
    return;
}

int hasNextAlpha(stationsADT stations){
    return (stations->iterAlpha != NULL);
}

void nextAlpha(stationsADT stations){
  if ( !hasNextAlpha(stations)) {
    fprintf(stderr, ERROR_ITER);
    exit(EXIT_FAILURE);
  } 
  stations->iterAlpha=stations->iterAlpha->tailAlpha;
  return;
}

char * getStationNameAlpha(stationsADT stations){
    if (stations->iterAlpha == NULL)
    {
        fprintf(stderr, ITER_NOT_ASSIGN_ERROR);
        exit(EXIT_FAILURE);
    }
    return stations->iterAlpha->stationName;
}

static int compareNameAndCount(const void * elem1,const void * elem2){
    int c;
    pNameIdAndCounter elemA = (pNameIdAndCounter)elem1;
    pNameIdAndCounter elemB = (pNameIdAndCounter)elem2;
    if((c = elemB->counter - elemA->counter) != 0){
        return c;
    }

    return strcasecmp(elemA->name,elemB->name);
}

static char * getMostPopularFromArrayAlpha(pStation station, size_t * amountOfTrips){
    qsort(station->mostPopularEndStations, station->sizeOfMostPopular, sizeof(*(station->mostPopularEndStations)),compareNameAndCount);
    if (station->mostPopularEndStations != NULL){
        *amountOfTrips = station->mostPopularEndStations->counter;
        return station->mostPopularEndStations->name;
    }
    *amountOfTrips=0;
    return NULL;
    
}

char * getMostPopularFromStationAlpha(stationsADT stations, size_t * amountOfTrips){
    if(stations->iterAlpha == NULL){
        printf(ITER_NOT_ASSIGN_ERROR);
        exit(EXIT_FAILURE);
    }
    char * toReturn = getMostPopularFromArrayAlpha(stations->iterAlpha,amountOfTrips);
    if (toReturn==NULL){
        return EMPTY_IDENTIFIER;
    }
    return toReturn;
}
