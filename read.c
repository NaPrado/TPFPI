#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "stationADT.h"
#include "read.h"

#define ESCAPE_CERO '\0'
#define SEMICOLON ";"
#define MEMBER 1
#define CASUAL 0 
#define YEAR_ALIGMENT 1900 
#define MONTH_ALIGMENT 1 
#define MAXCHARSPERLINE 80
#define CHARSBLOCK 10
#define MAXNAMELENGTH 20
#define MAX_COLS_ELEMS_STATIONS 4
#define DATE_ELEMS 6
#define DATE_DELIM "- :"



enum dateForm{
    year=0,
    month,
    day,
    hour,
    mins,
    secs,
};



static void getLine(char * str, FILE *stream) {
    int i = 0;
    for (char c;c==ESCAPE_CERO; i++, c=getc(stream))
    {
        if ((i%CHARSBLOCK)==0)
        {
            str=realloc(str,CHARSBLOCK+i);
        }
        *(str+i)=c;
    }
    str=realloc(str,i);
    return;
}

static struct tm saveDate(char * date){
    char * token = strtok(date,DATE_DELIM);
    struct tm moment;
    for (int i = 0; i < DATE_ELEMS ; i++){
        if(token!=NULL){
            switch (i){
            case year:
                moment.tm_year=atoi(token)-YEAR_ALIGMENT;
                break;
            case month:
                moment.tm_mon=atoi(token)-MONTH_ALIGMENT;
                break;
            case day:
                moment.tm_mday=atoi(token);
                break;
            case hour:
                moment.tm_hour=atoi(token);
                break;
            case mins:
                moment.tm_min=atoi(token);
                break;
            case secs:
                moment.tm_sec=atoi(token);
                break;
            default:
                break;
            }
        }
        token = strtok(NULL, DATE_DELIM);
    }
    return moment;
}


static void readCSVFileBikes(char const *argv[],stationsADT stations){
    errno=0;
    FILE * bikesFile = fopen( argv[1], "rt");
    if(errno != 0 && bikesFile==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados\n");
        exit (EXIT_FAILURE);
    }
    
    char * s = NULL;
    //libera la primer linea
    getLine(s,bikesFile);
    free(s);
    while (!feof(bikesFile)){
    s=NULL;
    getLine(s, bikesFile);
    struct tm startDate;
    struct tm endDate;
    int idStart, idEnd, isMember;
    char * token=strtok(s,SEMICOLON);
        for (int q = 0; q < MAX_COLS_ELEMS_BIKES; q++) {
            if (token != NULL) {
                switch (q) {
                    case dateStart:
                        startDate=saveDate(token);
                        break;
                    case startedId:
                        idStart=atoi(token);
                        break;
                    case dateEnd:
                        endDate=saveDate(token);
                        break;
                    case endedId:
                        idEnd=atoi(token);
                        break;
                    case member:
                        if (FORMATMON)
                        {
                            isMember=atoi(token);
                        }
                        else if (FORMATNYC)
                        {
                            isMember=(*token=='m');
                        }
                        break;
                    default:
                        break;
                }
            }
            token = strtok(NULL, SEMICOLON);
        }
        addRental(startDate,idStart,endDate,idEnd,isMember,stations);
    }
    fclose(bikesFile);
}

void readCSVFileStation(char const * argv[],stationsADT stations){
    if (FORMATMON && FORMATNYC)
    {
        perror("Ocurrio un error en compilacion, se debe aclarar en el makefile el formato de que ciudad a usar\n");
        exit (EXIT_FAILURE);
    }
    
    errno = 0;
    FILE * stationsFile = fopen( argv[2], "rt");
    if(errno != 0 || stationsFile==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones\n");
        exit (EXIT_FAILURE);
    }
    char * s = NULL;
    // Leer líneas desde el archivo
    getLine(s, stationsFile);
    while (!feof(stationsFile)){
        getLine(s, stationsFile);
        int id;
        char name[MAXNAMELENGTH]={0};
        char * token=strtok(s,SEMICOLON);
        for (int q = 0; q < MAX_COLS_ELEMS_STATIONS; q++) {
            if (token != NULL) {
                switch (q) {
                    case idStation:
                        // leo el id
                        id = atoi(token);
                        break;
                    case stationName:
                        // leo el name
                        strcpy(name,token);
                        break;
                    default:
                        break;
                }
            }
            token = strtok(NULL, SEMICOLON);  // Mueve la llamada a strtok fuera del switch
        }
        addStation(stations,name,id);
    }
    
    fclose(stationsFile);
    readCSVFileBikes(argv,stations);
    /* freeTree(tree); */
}