#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libreria HTML/htmlTable.h" 
#define DAYS_IN_WEEK 7

enum DAYS           
{
    saturday,     
    sunday,     
    monday,       
    tuesday,
    wednesday,     
    thursday,
    friday
} workday;

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
                                //si nahue pq hay q saber cual es el primer elem, de hecho, necesitas un firstCount aca para tu nueva forma de ordenar 

typedef struct stationCDT * stationADT;

static int dayOfWeek(int day, int month, int year){
    if (month < 3) {
        month += 12;
        year--;
    }
    int yearInCentury = year % 100;
    int century = year / 100 ;
    int h = ( day + (13 * ( month + 1 )) / 5 + yearInCentury + yearInCentury / 4 + century / 4 - 2 * century ) % 7;
    return h;
}

void countTrips(pRental rentalList, size_t * startedTrips, size_t * endedTrips){
    while(rentalList != NULL){
        startedTrips[dayOfWeek(rentalList->dateStart->tm_mday,rentalList->dateStart->tm_mon+1,rentalList->dateStart->tm_year+1900)]+=1;
        endedTrips[daysOfWeek(rentalList->dateEnd->tm_mday,rentalList->dateEnd->tm_mon+1,rentalList->dateEnd->tm_year+1900)]+=1;
        rentalList=rentalList->tail;
    }
    return;
}

void writeQ3(size_t * startedTrips, size_t * endedTrips){
    char ** weekDays={"Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday"};
    errno = 0;
    FILE * csvQ3 = fopen("query3.csv","wt");
    if(errno != 0 || csvQ3==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fputs("weekDay;startedTrips;endedTrips\n",csvQ3);
    htmlTable tablaQ3 = newTable("query3.html",3,"weekDay","startedTrips","endedTrips");
    for (size_t i = 2, j=i; i < DAYS_IN_WEEK+monday; i++)
    {
        if (i%friday==0)
        {
            j=saturday;
        }
        else{
            j++;
        }
        fscanf(csvQ3,"%s;%d;%d\n",*(weekDays+j),startedTrips[j],endedTrips[j]);
        char * sT, * eT;
        sprintf(sT,"%d",startedTrips);
        sprintf(eT,"%d",endedTrips);
        addHTMLRow(tablaQ3,*(weekDays+j),sT,eT);
    }
    closeHTMLTable(tablaQ3);
    fclose(csvQ3);
}

void query3(stationADT stations){
    size_t startedTrips[DAYS_IN_WEEK];
    size_t endedTrips[DAYS_IN_WEEK];
    pStation aux = stations->firstCount;
    while(aux != NULL && aux->totalAmountRentals != 0){
        countTrips(aux->oldestRental, startedTrips, endedTrips);
        aux=aux->tailCount;
    }
    writeQ3(startedTrips,endedTrips);
};
