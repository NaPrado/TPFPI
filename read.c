#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <math.h>
#include "stationADT.h"

#define MEMBER 1
#define CASUAL 0 

static __ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        // Si no hay un búfer asignado o el tamaño es 0, asignar un nuevo búfer
        *n = 10; // Puedes ajustar el tamaño inicial según tus necesidades
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Error de asignación de memoria
        }
    }

    size_t pos = 0;
    int c;

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        (*lineptr)[pos++] = c;

        // Verificar si necesitamos redimensionar el búfer
        if (pos >= *n - 1) {
            *n *= 2; // Duplicar el tamaño del búfer
            char *new_lineptr = (char *)realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                free(*lineptr);
                return -1; // Error de realocación de memoria
            }
            *lineptr = new_lineptr;
        }
    }

    if (pos == 0 && c == EOF) {
        return -1; // No se leyó ninguna línea y se alcanzó el final del archivo
    }

    (*lineptr)[pos] = '\0'; // Agregar el carácter nulo al final de la línea
    return pos; // Devolver el número de caracteres leídos
}

static void inicializerBikesMONFormat(char const *argv[],bst tree,stationADT station){
    errno=0;
    FILE * bikesMON = fopen( argv[1], "rt");
    if(errno != 0 && bikesMON==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Montreal\n");
        exit (EXIT_FAILURE);
    }
    
    char * s = NULL;
    size_t longitud = 0;
    // Leer líneas desde el archivo
    errno=0;
    if(getline(&s, &longitud, bikesMON)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de viajes realizados en Montreal\n");
        exit (EXIT_FAILURE);
    }
    free(s);
    while (!feof(bikesMON)){
    s=NULL;
    if(getline(&s, &longitud, bikesMON)==-1){
        perror("Ocurrio un error leyendo alguna linea del archivo de viajes realizados en Montreal\n");
        exit (EXIT_FAILURE);
    }
    struct tm startDate;
    struct tm endDate;
    int idStart, idEnd, isMember;
            // Formato: yyyy-mm-dd HH:mm:ss;idStart;yyyy-mm-dd HH:mm:ss;idEnd;isMember
    int result = sscanf(s, "%d-%d-%d %d:%d:%d;%d;%d-%d-%d %d:%d:%d;%d;%d",
                        &(startDate.tm_year), &(startDate.tm_mon), &(startDate.tm_mday),
                        &(startDate.tm_hour), &(startDate.tm_min), &(startDate.tm_sec),
                        &idStart,
                        &(endDate.tm_year), &(endDate.tm_mon), &(endDate.tm_mday),
                        &(endDate.tm_hour), &(endDate.tm_min), &(endDate.tm_sec),
                        &idEnd,
                        &isMember);
        //seteo de fechas
        startDate.tm_isdst = -1;
        startDate.tm_year=startDate.tm_year-1900;
        startDate.tm_mon=startDate.tm_mon-1, 
        endDate.tm_year=endDate.tm_year-1900;
        endDate.tm_mon=startDate.tm_mon-1;

        if (result == 15){ // La cadena se analizó correctamente, los valores están en las variables correspondientes.
            addRental(tree,startDate,idStart,endDate,idEnd,isMember,station);
        }
        else if (result!=0){// Hubo un problema al analizar la cadena
            printf("Error al analizar la cadena\n");
        }
        free(s);
    }
    fclose(bikesMON);
}

void inicializerMONFormat(char const * argv[],stationADT station){
    bst tree=newtree();
    errno = 0;
    FILE * stationsMON = fopen( argv[2], "rt");
    if(errno != 0 || stationsMON==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones de Montreal\n");
        exit (EXIT_FAILURE);
    }
    char * s = NULL;
    size_t longitud = 0;
    // Leer líneas desde el archivo
    errno=0;
    if(getline(&s, &longitud, stationsMON)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de estaciones de Montreall\n");
        exit (EXIT_FAILURE);
    }
    while (!feof(stationsMON)){
        if(getline(&s, &longitud, stationsMON)==-1){
            perror("Ocurrio un error leyendo alguna linea del archivo de estaciones de Montreall\n");
            exit (EXIT_FAILURE);
        }
        int id;
        char * token=strtok(s,";");
        for (int q = 0; q < 4; q++) {
            if (token != NULL && q < 2) {
                switch (q) {
                    case 0:
                        // leo el id
                        id = atoi(token);
                        break;
                    case 1:
                        // leo el name
                        addStation(station,token,tree,id);
                        break;
                    default:
                        break;
                }
            }
            token = strtok(NULL, ";");  // Mueve la llamada a strtok fuera del switch
        }
    }
    
    fclose(stationsMON);
    inicializerBikesMONFormat(argv,tree,station);
    orderByCount(station);
    freeTree(tree);
}

static void inicializerBikesNYCFormat(char const *argv[],bst tree,stationADT station){
    errno=0;
    FILE * bikesNYC = fopen( argv[1], "rt");
    if(errno != 0 && bikesNYC==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Nueva York\n");
        exit (EXIT_FAILURE);
    }
    
    char * s = NULL;
    size_t longitud = 0;
    errno=0;
    if(getline(&s, &longitud, bikesNYC)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de viajes realizados en Nueva York\n");
        exit (EXIT_FAILURE);
    }
    free(s);
    while (!feof(bikesNYC)){
        s=NULL;
        if(getline(&s, &longitud, bikesNYC)==-1){
            perror("Ocurrio un error leyendo alguna linea del archivo de viajes de Nueva York\n");
            exit (EXIT_FAILURE);
        }

        struct tm startDate;
        struct tm endDate;
        int idStart, idEnd;
        char isMember;
                // Formato: yyyy-mm-dd HH:mm:ss;idStart;yyyy-mm-dd HH:mm:ss;idEnd;rideable_type;member_casual
        int result = sscanf(s, "%d-%d-%d %d:%d:%d.000000;%d;%d-%d-%d %d:%d:%d.000000;%d;%*[^;];%c\n",
                            &(startDate.tm_year), &(startDate.tm_mon), &(startDate.tm_mday), 
                            &(startDate.tm_hour), &(startDate.tm_min), &(startDate.tm_sec),
                            &idStart,
                            &(endDate.tm_year), &(endDate.tm_mon), &(endDate.tm_mday),
                            &(endDate.tm_hour), &(endDate.tm_min), &(endDate.tm_sec),
                            &idEnd,
                            &isMember);
        //seteo de fechas
        startDate.tm_isdst = -1;
        startDate.tm_year=startDate.tm_year-1900;
        startDate.tm_mon=startDate.tm_mon-1, 
        endDate.tm_year=endDate.tm_year-1900;
        endDate.tm_mon=startDate.tm_mon-1;
        isMember=(isMember=='m')?MEMBER:CASUAL;

        if (result == 15) {
            // La cadena se analizó correctamente, los valores están en las variables correspondientes.
            addRental(tree,startDate,idStart,endDate,idEnd,isMember,station);
        } 
        else if (result!=0){// Hubo un problema al analizar la cadena
            printf("Error al analizar la cadena\n");
        }
        free(s);
    }
    fclose(bikesNYC);
}

void inicializerNYCFormat(char const * argv[],stationADT station){
    bst tree=newtree();
    errno = 0;
    FILE * stationsNYC = fopen( argv[2], "rt");
    if(errno != 0 || stationsNYC==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones de Nueva York\n");
        exit (EXIT_FAILURE);
    }
    char * s = NULL;
    size_t longitud = 0;
    // Leer líneas desde el archivo
    errno=0;
    if(getline(&s, &longitud, stationsNYC)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de estaciones de Nueva York\n");
        exit (EXIT_FAILURE);
    }
    while (!feof(stationsNYC)){
        if (getline(&s, &longitud, stationsNYC)==-1){
            perror("Ocurrio un error leyendo alguna linea del archivo de estaciones de Nueva York\n");
            exit (EXIT_FAILURE);
        }
        char * name;
        int id;
        char * token=strtok(s,";");
        for (int q = 0; q < 4; q++) {
            if (token != NULL) {
                switch (q) {
                    case 0:
                        // leo el name
                        name=token;
                        break;
                    case 3:
                        // leo el id
                        id = atoi(token);
                        addStation(station,name,tree,id);
                        break;
                    default:
                        break;
                }
            }
            token = strtok(NULL, ";");  // Mueve la llamada a strtok fuera del switch
        }
    }
    fclose(stationsNYC);
    inicializerBikesNYCFormat(argv,tree,station);
    orderByCount(station);
    freeTree(tree);
}