#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "stationADT.h"
#include "read.h"


#define YEAR_ALIGMENT 1900 
#define MONTH_ALIGMENT 1 
#define MAX_CHARS_PER_LINE 200
#define SET_ERRNO 0
#define READ_TEXT "rt"



enum dateForm{
    year=0,
    month,
    day,
    hour,
    mins,
    secs,
    lastElemDates,
};

//assigna la fecha en formato yyyy-MM-dd HH:mm:ss a un struct tm
static struct tm saveDate(char * date){
    char * token = strtok(date,DATE_DELIM);
    struct tm moment;
    for (int i = 0; token!=NULL; i++){
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
        token = strtok(NULL, DATE_DELIM);
    }
    moment.tm_isdst=DAYLIGHT_SAVING_TIME;
    return moment;
}


static void readCSVFileBikes(char const *argv[],stationsADT stations){
    errno=SET_ERRNO;
    FILE * bikesFile = fopen( argv[BIKES_FILE], READ_TEXT);
    if(errno != SET_ERRNO && bikesFile==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados\n");
        exit (EXIT_FAILURE);
    }
    
    char s[MAX_CHARS_PER_LINE];
    //libera la primer linea
    fgets(s,MAX_CHARS_PER_LINE,bikesFile);
    while (fgets(s,MAX_CHARS_PER_LINE, bikesFile)){
        struct tm startDate;
        struct tm endDate;
        char * sDate,* eDate;
        int idStart, idEnd, isMember;
        char * token=strtok(s,DELIM);
        if (token!=NULL){
            for (int q = 0; token!=NULL; q++){            
                    switch (q) {
                        case dateStart:
                            sDate=token;
                            break;
                        case startedId:
                            idStart=atoi(token);
                            break;
                        case dateEnd:
                            eDate=token;
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
                token = strtok(NULL, DELIM);
            }
            //se guarrdan las fechas en un struct tm
            startDate=saveDate(sDate);
            endDate=saveDate(eDate);
            //se llama a la funcion que guarda datos de alquileres
            addRental(startDate,idStart,endDate,idEnd,isMember,stations);  
        }          
    }
    fclose(bikesFile);
}

void readCSVFileStation(char const * argv[],stationsADT stations){
    //si no se selecciona ninguna flag o si se selecconan ambas, usando el makefile no deberia suceder
    if ((FORMATMON == 1 && FORMATNYC == 1)||(FORMATMON == 0 && FORMATNYC == 0))
    {
        perror("Ocurrio un error en compilacion, se debe aclarar en el makefile el formato de que ciudad a usar\n");
        exit (EXIT_FAILURE);
    }
    errno = SET_ERRNO;
    FILE * stationsFile = fopen( argv[STATION_FILE], READ_TEXT);
    if(errno != SET_ERRNO || stationsFile==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones\n");
        exit (EXIT_FAILURE);
    }
    char s[MAX_CHARS_PER_LINE];
    // Leer líneas desde el archivo
    fgets(s,MAX_CHARS_PER_LINE,stationsFile);
    while (fgets(s,MAX_CHARS_PER_LINE,stationsFile)){
        int id;
        char * name;
        char * token=strtok(s,DELIM);
        for (int q = 0; token!=NULL ; q++) {
                switch (q) {
                    case idStation:
                        // leo el id
                        id = atoi(token);
                        break;
                    case stationName:
                        // leo el name
                        name=token;
                        break;
                    default:
                        break;
                }
            // Mueve la llamada a strtok fuera del switch
            token = strtok(NULL, DELIM);  
        }
        //se llama a la funcion que guarda datos de estaciones
        addStation(stations,name,id); 
    }
    fclose(stationsFile);
    toBeginAlpha(stations);
    readCSVFileBikes(argv,stations);
}