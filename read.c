#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "stationADT.h"
#include "read.h"

#define ESCAPE_N '\n'
#define SEMICOLON ";"
#define MEMBER 1
#define CASUAL 0 
#define YEAR_ALIGMENT 1900 
#define MONTH_ALIGMENT 1 
#define MAXCHARSPERLINE 200
#define CHARSBLOCK 10
#define MAXNAMELENGTH 20
#define DATE_ELEMS 6
#define DATE_DELIM "- :"



enum dateForm{
    year=0,
    month,
    day,
    hour,
    mins,
    secs,
    lastElemDates,
};

char* copyString(const char * origin) {
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
    return moment;
}


static void readCSVFileBikes(char const *argv[],stationsADT stations){
    errno=0;
    FILE * bikesFile = fopen( argv[1], "rt");
    if(errno != 0 && bikesFile==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados\n");
        exit (EXIT_FAILURE);
    }
    
    char s[MAXCHARSPERLINE];
    //libera la primer linea
    fgets(s,MAXCHARSPERLINE,bikesFile);
    while (!feof(bikesFile)){
        fgets(s,MAXCHARSPERLINE, bikesFile);
        struct tm startDate;
        struct tm endDate;
        char * sDate,* eDate;
        int idStart, idEnd, isMember;
        char * token=strtok(s,SEMICOLON);
        if (token!=NULL)
        {
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
                token = strtok(NULL, SEMICOLON);
            }
            startDate=saveDate(sDate);
            endDate=saveDate(eDate);
            addRental(startDate,idStart,endDate,idEnd,isMember,stations);  
        }          
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
    char s[MAXCHARSPERLINE];
    // Leer líneas desde el archivo
    fgets(s,MAXCHARSPERLINE,stationsFile);
    while (!feof(stationsFile)){
        fgets(s,MAXCHARSPERLINE,stationsFile);
        int id;
        char * name;
        char * token=strtok(s,SEMICOLON);
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
            token = strtok(NULL, SEMICOLON);  // Mueve la llamada a strtok fuera del switch
        }
        addStation(stations,name,id);
    }
    fclose(stationsFile);
    readCSVFileBikes(argv,stations);
}