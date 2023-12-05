#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#define BLOQUECHARS 10
#include "stationADT.h"



stationsIdBST tree=NULL;

stationADT inicializerBikesMONFormat(char const argv[],stationADT newStation){
    FILE * bikesMON = fopen( argv[0], "rt");
    if(errno != 0 || bikesMON==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Montreal\n");
        exit (1);
    }
    readIndex(bikesMON);
    char * s=NULL;
    while (!feof(bikesMON)){
        s=realloc(s,sizeof(char)*BLOQUECHARS);
        int i=0;
        for (int j=0, c ; c=fgetc(bikesMON) != '\n' ; i++){
            if (i%BLOQUECHARS==0)
            {
                j++;
                s=realloc(s,sizeof(char)*BLOQUECHARS*j);
            }
            *(s+i)=c;
        }
        s=realloc(s,sizeof(char)*(i+1));
        *(s+i)="\0";
        int idStart, idEnd;
        for (int q = 0; q < 5; q++){
            char * token=strtok(s,";");
            if (token != NULL){
                switch (q){
                    case 0:
                        //leo el start_date
                        assignDate(token);
                        break;
                    case 1:
                        //leo el emplacement_pk_start
                        idStart=atoi(token);
                        break;
                    case 2:
                        //leo el end_date
                        assignDate(token);
                        break;
                    case 3:
                        //leo el emplacement_pk_end
                        idEnd=atoi(token);
                        break;
                    case 4:
                        //leo el is_member Si el usuario del alquiler es miembro del sistema de alquiler (0 si no es miembro, 1 si lo es)
                        int member=( *token == '1' );//hay que guardarlo en el struct no en member
                        break;
                    default:
                        break;
                }
            }
        }
    }
    free(s);
    fclose(bikesMON);
}