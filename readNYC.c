#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <math.h>
#include "stationADT.h"
#include "readNYC.h"

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
                free(*lineptr); //NO SE SI ES NECESARIO
                return -1; 
            }
            *lineptr = new_lineptr;
        }
    }

    if (pos == 0 && c == EOF) {
        //free(*lineptr);
        return -1; // No se leyó ninguna línea y se alcanzó el final del archivo
    }

    (*lineptr)[pos] = '\0'; 
    return pos; 
}

static void inicializerBikesNYCFormat(char const *argv[],stationsIdBST tree,stationADT station){
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
        getline(&s, &longitud, bikesNYC); //VER SI ES POSIBLE Q FALLE
        struct tm * startDate=calloc(1,sizeof(struct tm));
        struct tm * endDate=calloc(1,sizeof(struct tm));
        if(startDate == NULL || endDate == NULL){
            printf("Error al allocar memoria\n");
            exit(EXIT_FAILURE);
        }

        int idStart, idEnd;
        char isMember;
                // Formato: yyyy-mm-dd HH:mm:ss;idStart;yyyy-mm-dd HH:mm:ss;idEnd;rideable_type;member_casual
        int result = sscanf(s, "%d-%d-%d %d:%d:%d.000000;%d;%d-%d-%d %d:%d:%d.000000;%d;%*[^;];%c\n",
                            &startDate->tm_year, &startDate->tm_mon, &startDate->tm_mday, &startDate->tm_hour, &startDate->tm_min, &startDate->tm_sec,
                            &idStart,
                            &endDate->tm_year, &endDate->tm_mon, &endDate->tm_mday,&endDate->tm_hour, &endDate->tm_min, &endDate->tm_sec,
                            &idEnd,&isMember);
        //seteo de fechas
        startDate->tm_year=startDate->tm_year-1900;
        startDate->tm_mon=startDate->tm_mon-1, 
        endDate->tm_year=endDate->tm_year-1900;
        endDate->tm_mon=startDate->tm_mon-1;
                        
        if (result == 15) {
            // La cadena se analizó correctamente, los valores están en las variables correspondientes.
            addRental(tree,startDate,idStart,endDate,idEnd,isMember,station);
        } else {
            // Hubo un problema al analizar la cadena
            printf("Error al analizar la cadena\n");
        }
            free(s);
    }
    fclose(bikesNYC);
}

void inicializerNYCFormat(char const * argv[],stationADT station){
    stationsIdBST tree=NULL;
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
        getline(&s, &longitud, stationsNYC);
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
    inicializerBikesNYCFormat(argv,newStation,tree);
    orderByCount(station);
    freeTree(tree);
}