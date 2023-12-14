#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "htmlTable.h"
#include "writeCSV.h"
#include "stationADT.h"
#define QUERY_2_DATE_FORMAT_LONGITUD 20
#define BLOCK 10
#define FIRST 0
#define SECOND 1
#define THIRD 2
#define STRMAXLONG 20

static int countDigit(int num) {
    int count = 0;

    // Manejar caso especial para el número 0
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

/* char * intToAlocStr(int num){
    char * str=NULL;
    int i=0;
    while (num!=0){
        if (i%BLOCK==0){
            str=realloc(str,BLOCK+i);
        }
        *(str+i)=(num%10)+'0';
        num/=10;
        i++;
    }
    *(str+i)='\0';
    return str;
}
 */



static void writeQ1Rec(stationsADT stations, htmlTable tablaQ1, FILE * csvQ1){
    size_t members=getMembersQ1(stations);
    size_t casuals=getCasualsQ1(stations);
    size_t total=getTotalQ1(stations);

    char * membersStr=calloc(1,sizeof(char)*(countDigit(members)+1));
    char *casualsStr=calloc(1,sizeof(char)*(countDigit(casuals)+1));
    char *totalStr=calloc(1,sizeof(char)*(countDigit(total)+1));

    sprintf(membersStr,"%zu",members);
    sprintf(casualsStr,"%zu",casuals);
    sprintf(totalStr,"%zu",total);

    char * stationName=getStationNameQ1(stations);

    addHTMLRow(tablaQ1,stationName,membersStr,casualsStr,totalStr);
    writeRowQ1(csvQ1,stationName,membersStr,casualsStr,totalStr);
    free(membersStr);
    free(casualsStr);
    free(totalStr);
    if (!hasNextQ1(stations)){
        return;
    }
    nextQ1(stations);
    writeQ1Rec(stations,tablaQ1,csvQ1);
    return;
}

void query1(stationsADT stations){
    FILE * csvQ1=newFile("query1.csv");
    writeHeaderQ1(csvQ1);
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    toBeginQ1(stations);
    writeQ1Rec(stations,tablaQ1,csvQ1);
    closeHTMLTable(tablaQ1);
    fclose(csvQ1);
}

static void writeQ2Rec(stationsADT stations, htmlTable tablaQ2, FILE * csvQ2){
    while (hasNextQ2(stations))
    {
        if (hasRentsQ2(stations))
        {
            char * stationNamestr = strcpy(stationNamestr,getStationNameQ2(stations));
            char * stationNameEndStr = strcpy(stationNameEndStr,getOldestRentalStationNameEndQ2(stations));
            struct tm startDate = getOldestRentalStartDateQ2(stations);
            char * startDateStr = calloc(1,sizeof(char)*QUERY_2_DATE_FORMAT_LONGITUD);
            strftime(startDateStr,QUERY_2_DATE_FORMAT_LONGITUD,"%d/%m/%Y %H:%M",&(startDate));
            addHTMLRow(tablaQ2,stationNamestr,stationNameEndStr,startDateStr);
            writeRowQ2(csvQ2,stationNamestr,stationNameEndStr,startDateStr);
            free(startDateStr);
        }
        nextQ2(stations);
    }
}

void query2(stationsADT stations){

    errno = 0;
    FILE * csvQ2 = newFile("query2.csv");
    writeHeaderQ2(csvQ2);
    htmlTable tablaQ2 = newTable("query2.html",3,"bikeStation","bikeEndStation","oldestDateTime");
    toBeginQ2(stations);
    writeQ2Rec(stations,tablaQ2,csvQ2);
    closeHTMLTable(tablaQ2);
    fclose(csvQ2);
}

void query3(stationsADT stations){
    size_t * startedTrips=getStartedTripsQ3(stations);
    size_t * endedTrips=getEndedTripsQ3(stations);
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
        /* free(sT);
        free(eT); */
    }
    closeHTMLTable(tablaQ3);
    fclose(csvQ3);
}

void query4(stationsADT stations){
    FILE * csvQ4 = newFile("query4.csv");
    writeHeaderQ4(csvQ4);
    htmlTable tablaQ4 = newTable("query4.html",3,"bikeStation","mostPopRouteEndStation","mostPopRouteTrips");
    toBeginQ4(stations);
    char * mostPopularName;
    size_t amountOfTrips;
    while(hasNextQ4(stations)){
        mostPopularName = getMostPopularFromStationQ4(stations,&amountOfTrips);
        char * name =getStationNameQ4(stations);
        char * amountOfTripsStr=NULL;
        sscanf(amountOfTripsStr,"%lu",&amountOfTrips);
        writeRowQ4(csvQ4,name, mostPopularName, amountOfTripsStr);
        addHTMLRow(tablaQ4,name, mostPopularName, amountOfTripsStr);
        free(amountOfTripsStr);
        nextQ4(stations);
    }
}

/* void query5(stationsADT stations){
    FILE * csvQ5 = newFile("query5.csv");
    writeHeaderQ4(csvQ5);
    htmlTable tablaQ5 = newTable("query5.html",4,"month","loopsTop1St","loopsTop2St","loopsTop3St");
    char * months[MONTHS_IN_YEAR] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    char * topThree[3];
    for(int i = 0; i<MONTHS_IN_YEAR; i++){
        getTopThreeCircularRentalStationsByMonth(stations, i,topThree);
        writeRowQ5(csvQ5,months[i],topThree[FIRST],topThree[SECOND],topThree[THIRD]);
        addHTMLRow(tablaQ5,months[i],topThree[FIRST],topThree[SECOND],topThree[THIRD]);
    }
} */