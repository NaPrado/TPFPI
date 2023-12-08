#include <stdio.h>
#include <stdlib.h>
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
        startedTrips[dayOfWeek(rentalList->dateStart)]+=1;
        endedTrips[daysOfWeek(rentalList->dateEnd)]+=1;
        rentalList=rentalList->tail;
    }
    return;
}

void writeQ3(size_t * startedTrips, size_t endedTrips){
    //imprimir "titulo"
    //algo q imprima:
    // "Monday;%d;%d\nTuesday;%d;%d\nWednesday;%d;%d\nThursday;%d;%d\nFriday;%d;%d\nSaturday;%d;%d\nSunday;%d;%d\n", startedtrips[monday],endedTrips[monday],startedtrips[tuesday],endedTrips[tuesday],startedtrips[wednesday],endedTrips[wednesday],startedtrips[thursday],endedTrips[thursday],startedtrips[friday],endedTrips[friday],startedtrips[saturday],endedTrips[saturday],startedtrips[sunday],endedTrips[sunday]

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
