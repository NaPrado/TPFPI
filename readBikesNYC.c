#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include "stationADT.h"


#include <stdio.h>
#include <stdlib.h>

__ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Error de asignación de memoria
        }
    }

    size_t pos = 0;
    int c;

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        (*lineptr)[pos++] = c;

        if (pos >= *n - 1) {
            *n *= 2;
            char *new_lineptr = (char *)realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                free(*lineptr); // Liberar el búfer original
                return -1;      // Error de realocación de memoria
            }
            *lineptr = new_lineptr;
        }
    }

    if (pos == 0 && c == EOF) {
        //free(*lineptr); // Liberar el búfer original
        return -1;      // No se leyó ninguna línea y se alcanzó el final del archivo
    }

    (*lineptr)[pos] = '\0';
    return pos; // Devolver el número de caracteres leídos
}




/* stationADT */ void inicializerBikesMONFormat(char const *argv[]/* ,stationADT newStation */){
    errno=0;
    FILE * bikesNYC = fopen( argv[1], "rt");
    if(errno != 0 && bikesNYC==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Montreal\n");
        exit (1);
    }
    
    char * s = NULL;
    size_t longitud = 0;
    // Leer líneas desde el archivo
    errno=0;
    if(getline(&s, &longitud, bikesNYC)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de viajes realizados en Nueva York\n");
        exit (1);
    }
/*     char * format="started_at;start_station_id;ended_at;end_station_id;rideable_type;member_casual\n";
    if (strcmp(s,format)!=0){
        printf("Formato incorrecto, su formato es:\n%s\n y el correcto es:\n%s",s,format);
        exit(1);
    } */
    free(s);
    while (!feof(bikesNYC)){
        s=NULL;
        getline(&s, &longitud, bikesNYC);
        struct tm startDate, endDate;
        int idStart, idEnd;
   /*      char * z=NULL; */
        char isMember;
                // Formato: yyyy-mm-dd HH:mm:ss;idStart;yyyy-mm-dd HH:mm:ss;idEnd;rideable_type;member_casual
        int result = sscanf(s, "%d-%d-%d %d:%d:%d.000000;%d;%d-%d-%d %d:%d:%d.000000;%d;%*[^;];%c\n",
                            &startDate.tm_year, &startDate.tm_mon, &startDate.tm_mday, &startDate.tm_hour, &startDate.tm_min, &startDate.tm_sec,
                            &idStart,
                            &endDate.tm_year, &endDate.tm_mon, &endDate.tm_mday,&endDate.tm_hour, &endDate.tm_min, &endDate.tm_sec,
                            &idEnd,&isMember);
        
/*         strtok(z,";");
        result += sscanf(z," %c\n",&isMember);
        strtok(z,"\n");
        free(z);
 */
        printf("%d",result);
        if (result == 15) {
            // La cadena se analizó correctamente, los valores están en las variables correspondientes.
            printf("%d-%02d-%02d %02d:%02d:%02d;\t", startDate.tm_year, startDate.tm_mon, startDate.tm_mday,
                   startDate.tm_hour, startDate.tm_min, startDate.tm_sec);
            printf("%d;\t", idStart);
            printf("%d-%02d-%02d %02d:%02d:%02d;\t", endDate.tm_year, endDate.tm_mon, endDate.tm_mday,
                   endDate.tm_hour, endDate.tm_min, endDate.tm_sec);
            printf("%d;\t", idEnd);
            printf("%d;\n", isMember=='c');
        } else {
            // Hubo un problema al analizar la cadena
            printf("Error al analizar la cadena\n");
        }
            free(s);
    }
    fclose(bikesNYC);
}

int main(int argc, char const *argv[])
{
    time_t t=time(NULL);
    /* stationADT newStation;
    newStation=calloc(1,sizeof(stationADT)); */
    inicializerBikesMONFormat(argv/* ,newStation */);
    /* free(newStation); */
    printf("%ld\n",time(NULL)-t);
    return 0;
}
