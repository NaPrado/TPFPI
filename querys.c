#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "querys.h"
#include "htmlTable.h"
#include "writeCSV.h"
#include "stationADT.h"
#define QUERY_2_DATE_FORMAT_LONGITUD 20
#define BLOCK 10
#define FIRST 0
#define SECOND 1
#define THIRD 2
#define STRMAXLONG 20
#define ERROR_ALLOCATION "Error alocando memoria\n"
#define SET_ERRNO 0
#define AMOUNT_OF_TRIPS_DIGIT 10

static int countDigit(int num) {
    int count = 0;
    // Caso especial para el número 0
    if (num == 0) {
        return 1;
    }
    // Contar los dígitos del número
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

static void writeQ1Rec(stationsADT stations, htmlTable tablaQ1, FILE * csvQ1){
    while (hasNextCount(stations)){

        size_t members=getMembersCount(stations);
        size_t casuals=getCasualsCount(stations);
        size_t total=getTotalCount(stations);
        
        char * membersStr=calloc(1,sizeof(char)*(countDigit(members)+1));
        char * casualsStr=calloc(1,sizeof(char)*(countDigit(casuals)+1));
        char * totalStr=calloc(1,sizeof(char)*(countDigit(total)+1));
        char * stationName=getStationNameCount(stations);

        if (membersStr==NULL || casualsStr==NULL || totalStr ==NULL){
            perror(ERROR_ALLOCATION);
            exit(EXIT_FAILURE);
        }
        

        sprintf(membersStr,"%zu",members);
        sprintf(casualsStr,"%zu",casuals);
        sprintf(totalStr,"%zu",total);

        addHTMLRow(tablaQ1,stationName,membersStr,casualsStr,totalStr);
        writeRowQ1(csvQ1,stationName,membersStr,casualsStr,totalStr);

        free(membersStr);
        free(casualsStr);
        free(totalStr);
        
        nextCount(stations);
        }
    return;
}

void query1(stationsADT stations){
    FILE * csvQ1=newFile("query1.csv");
    writeHeaderQ1(csvQ1);
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    toBeginCount(stations);
    writeQ1Rec(stations,tablaQ1,csvQ1);
    closeHTMLTable(tablaQ1);
    fclose(csvQ1);
}

static void writeQ2Rec(stationsADT stations, htmlTable tablaQ2, FILE * csvQ2){
    while (hasNextAlpha(stations))
    {
        if (hasRentsAlpha(stations))
        {
            char * stationNamestr = getStationNameAlpha(stations);
            char * stationNameEndStr = getOldestRentalStationNameEndAlpha(stations);
            struct tm startDate = getOldestRentalStartDateAlpha(stations);
            char * startDateStr = calloc(1,sizeof(char)*QUERY_2_DATE_FORMAT_LONGITUD);
            if (startDateStr==NULL){
                perror(ERROR_ALLOCATION);
                exit(EXIT_FAILURE);
            }
            
            strftime(startDateStr,QUERY_2_DATE_FORMAT_LONGITUD,"%d/%m/%Y %H:%M",&(startDate));
            addHTMLRow(tablaQ2,stationNamestr,stationNameEndStr,startDateStr);
            writeRowQ2(csvQ2,stationNamestr,stationNameEndStr,startDateStr);
            free(startDateStr);
        }
        nextAlpha(stations);
    }
}

void query2(stationsADT stations){
    FILE * csvQ2 = newFile("query2.csv");
    writeHeaderQ2(csvQ2);
    htmlTable tablaQ2 = newTable("query2.html",3,"bikeStation","bikeEndStation","oldestDateTime");
    toBeginAlpha(stations);
    writeQ2Rec(stations,tablaQ2,csvQ2);
    closeHTMLTable(tablaQ2);
    fclose(csvQ2);
}

void query3(stationsADT stations){
    size_t * startedTrips=getStartedTrips(stations);
    size_t * endedTrips=getEndedTrips(stations);
    char * weekDays[]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    errno = 0;
    FILE * csvQ3 = newFile("query3.csv");
    writeHeaderQ3(csvQ3);
    htmlTable tablaQ3 = newTable("query3.html",3,"weekDay","startedTrips","endedTrips");
    for (size_t i = 0; i < DAYS_IN_WEEK; i++)
    {
        char sT[STRMAXLONG];
        sprintf(sT,"%lu",startedTrips[i]);
        char eT[STRMAXLONG];
        sprintf(eT,"%lu",endedTrips[i]);
        writeRowQ3(csvQ3,*(weekDays+i),sT,eT);
        addHTMLRow(tablaQ3,*(weekDays+i),sT,eT);
        }
    closeHTMLTable(tablaQ3);
    fclose(csvQ3);
}

void query4(stationsADT stations){
    FILE * csvQ4 = newFile("query4.csv");
    writeHeaderQ4(csvQ4);
    htmlTable tablaQ4 = newTable("query4.html",3,"bikeStation","mostPopRouteEndStation","mostPopRouteTrips");
    toBeginAlpha(stations);
    char * mostPopularName;
    size_t amountOfTrips;
    while(hasNextAlpha(stations)){
        mostPopularName = getMostPopularFromStationAlpha(stations,&amountOfTrips);
        char * name =getStationNameAlpha(stations);
        char amountOfTripsStr[AMOUNT_OF_TRIPS_DIGIT];
        sprintf(amountOfTripsStr,"%lu",amountOfTrips);
        writeRowQ4(csvQ4,name, mostPopularName, amountOfTripsStr);
        addHTMLRow(tablaQ4,name, mostPopularName, amountOfTripsStr);
        nextAlpha(stations);
    }
    closeHTMLTable(tablaQ4);
    fclose(csvQ4);
    return;
}