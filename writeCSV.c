#include <stdio.h>
#include <stdlib.h>
#include "htmlTable.h"
#include "writeCSV.h"

#define ESCAPE_N "\n"
#define TOPQ1 4
#define LASTQ1 (TOPQ1-1)
#define TOPQ2 3
#define LASTQ2 (TOPQ2-1)
#define TOPQ3 3
#define LASTQ3 (TOPQ3-1)
#define TOPQ4 3
#define LASTQ4 (TOPQ4-1)
#define WRITETEXT "wt"
#define SET_ERRNO 0



FILE * newFile(char * name){
    errno = SET_ERRNO;
    FILE * file=fopen(name,WRITETEXT);
    if(errno != SET_ERRNO || file==NULL){
        perror("Ocurrio un error mientras se creaba algun archivo\n");
        exit (EXIT_FAILURE);
    }
    return file;
}

void writeHeaderQ1(FILE * stream){
    fputs("bikeStation;memberTrips;casualTrips;allTrips\n",stream);
}

void writeRowQ1(FILE * stream,char * name,char * memberTrips,char * casualTrips,char * allTrips){
    char * info[]={name,memberTrips,casualTrips,allTrips};
    for (size_t i = 0; i < TOPQ1; i++)
    {
        fputs(info[i],stream);
        switch (i)
        {
        case LASTQ1:
            fputs(ESCAPE_N,stream);
            break;
        
        default:
            fputs(DELIM,stream);
            break;
        }
    }
}

void writeHeaderQ2(FILE * stream){
    fputs("bikeStation;bikeEndStation;oldestDateTime\n",stream);
}

void writeRowQ2(FILE * stream,char * name,char * nameEnd,char * oldestDate){
    char * info[]={name,nameEnd,oldestDate};
    for (size_t i = 0; i < TOPQ2; i++)
    {
        fputs(info[i],stream);
        switch (i)
        {
        case LASTQ2:
            fputs(ESCAPE_N,stream);
            break;
        
        default:
            fputs(DELIM,stream);
            break;
        }
    }
}

void writeHeaderQ3(FILE * stream){
    fputs("weekDay;startedTrips;endedTrips\n",stream);
}

void writeRowQ3(FILE * stream,char * weekday,char * startedTrips,char * endedTrips){
    char * info[]={weekday,startedTrips,endedTrips};
    for (size_t i = 0; i < TOPQ3; i++)
    {
        fputs(info[i],stream);
        switch (i)
        {
        case LASTQ3:
            fputs(ESCAPE_N,stream);
            break;
        
        default:
            fputs(DELIM,stream);
            break;
        }
    }
}

void writeHeaderQ4(FILE * stream){
    fputs("bikeStation;mostPopRouteEndStation;mostPopRouteTrips\n",stream);
}

void writeRowQ4(FILE * stream,char * name,char * mostPopRouteEndStation,char * mostPopRouteTrips){
    char * info[]={name,mostPopRouteEndStation,mostPopRouteTrips};
    for (size_t i = 0; i < TOPQ4; i++)
    {
        fputs(info[i],stream);
        switch (i)
        {
        case LASTQ4:
            fputs(ESCAPE_N,stream);
            break;
        
        default:
            fputs(DELIM,stream);
            break;
        }
    }
}