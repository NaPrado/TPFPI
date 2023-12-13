#include <string.h>
#include <strings.h>
#include "htmlTable.h"
#include "stationADT.h"
#define ESCAPE_CERO_SPACE 1
#define QUERY_2_DATE_FORMAT_LONGITUD 20

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

static void writeQ1Rec(stationsADT stations, htmlTable tablaQ1, FILE * csvQ1){
    size_t members=getAmountRentalsByMembersCount(stations);
    size_t casuals=getAmountRentalsByCasualsCount(stations);
    size_t total=getTotalAmountRentalsCount(stations);
    char * membersStr=calloc(1,(countDigit(members)+ESCAPE_CERO_SPACE));
    char * casualsStr=calloc(1,(countDigit(casuals)+ESCAPE_CERO_SPACE));
    char * totalStr=calloc(1,(countDigit(total)+ESCAPE_CERO_SPACE));
    
    sprintf(membersStr,"%zu",members);
    sprintf(casualsStr,"%zu",casuals);
    sprintf(totalStr,"%zu",total);
    char * stationName = getStationNameCount(stations);
    addHTMLRow(tablaQ1,stationName,members,casuals,total);
    fprintf(csvQ1,"%s;%ld;%ld;%ld\n",stationName,members,casuals,total);
    free(membersStr);
    free(casualsStr);
    free(totalStr);

    if (!hasNextCount(stations)){
        return;
    }
    nextCount(stations);
    writeQ1Rec(stations,tablaQ1,csvQ1);
    return;
}

void query1(stationsADT stations){
    orderByCount(stations);
    errno = 0;
    FILE * csvQ1 = fopen("query1.csv","wt");
    if(errno != 0 || csvQ1==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (EXIT_FAILURE);
    }
    fprintf(csvQ1,"bikeStation;memberTrips;casualTrips;allTrips\n");
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    

    toBeginCount(stations);
    if (!hasFirstCount(stations))
    {
        perror("No hay estaciones ingresadas\n");
        exit (EXIT_FAILURE);
    }
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
            strftime(startDateStr,QUERY_2_DATE_FORMAT_LONGITUD,"%d/%m/%Y %H:%M",&(startDate));
            addHTMLRow(tablaQ2,stationNamestr,stationNameEndStr,startDateStr);
            fprintf(csvQ2,"%s;%s;%s\n", stationNamestr, stationNameEndStr, startDateStr);
            free(startDateStr);
        }
        nextCount(stations);
    }
}

void query2(stationsADT stations){
    errno = 0;
    FILE * csvQ2 = fopen("query2.csv","wt");
    if(errno != 0 || csvQ2==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (EXIT_FAILURE);
    }
    fputs("bikeStation;bikeEndStation;oldestDateTime\n",csvQ2);
    htmlTable tablaQ2 = newTable("query2.html",3,"bikeStation","bikeEndStation","oldestDateTime");
    toBeginAlpha(stations);
    if (!hasFirstAlpha(stations))
    {
        perror("No hay estaciones ingresadas\n");
        exit (EXIT_FAILURE);
    }
    writeQ2Rec(stations,tablaQ2,csvQ2);
    closeHTMLTable(tablaQ2);
    fclose(csvQ2);
}

void query3(stationsADT stations){
    size_t * startedTrips=getStartedTrips(stations);
    size_t * endedTrips=getEndedTrips(stations);
    char * weekDays[]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    errno = 0;
    FILE * csvQ3 = fopen("query3.csv","wt");
    if(errno != 0 || csvQ3==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (EXIT_FAILURE);
    }
    fputs("weekDay;startedTrips;endedTrips\n",csvQ3);
    htmlTable tablaQ3 = newTable("query3.html",3,"weekDay","startedTrips","endedTrips");
    for (size_t i = 0; i < DAYS_IN_WEEK; i++)
    {
        fprintf(csvQ3,"%s;%lu;%lu\n",*(weekDays+i),startedTrips[i],endedTrips[i]);
        char sT[30];
        char eT[30];
        sprintf(sT,"%lu",startedTrips[i]);
        sprintf(eT,"%lu",endedTrips[i]);
        addHTMLRow(tablaQ3,*(weekDays+i),sT,eT);
    }
    closeHTMLTable(tablaQ3);
    fclose(csvQ3);
}

void query4(stationsADT stations){
    toBeginAlpha(stations);
    char * mostPopularName;
    size_t amountOfTrips;
    writeQ4Title();
    while(hasNextAlpha(stations)){
        mostPopularName = getMostPopularFromStationInAlphaOrder(stations,&amountOfTrips);
        writeQ4(getStationNameAlpha(stations), mostPopularName, amountOfTrips);
        nextAlpha(stations);
    }
}

void query5(stationsADT stations){
    char * months[MONTHS_IN_YEAR] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    char ** topThree;
    writeQ5Title();
    for(int i = 0; i<MONTHS_IN_YEAR; i++){
        topThree=getTopThreeCircularRentalStationsByMonth(stations, i);
        writeQ5(months[i],topThree);
    }
}