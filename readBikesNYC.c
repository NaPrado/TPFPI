#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#define BLOQUECHARS 10
#include "stationADT.h"

stationsIdBST tree=NULL;

stationADT inicializerMONFormat(char const argv[],stationADT newStation){
    FILE * bikesNYC = fopen( argv[0], "r");
    if(errno != 0){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Nueva York\n");
        exit (1);
    }
    readIndex(bikesNYC);
    char * s=NULL;
    while (!feof(bikesNYC)){
        s=realloc(s,sizeof(char)*BLOQUECHARS);
        int i=0;
        for (int j=0, c ; c=fgetc(bikesNYC) != "\n" ; i++){
            if (i%BLOQUECHARS==0)
            {
                j++;
                s=realloc(s,sizeof(char)*BLOQUECHARS*j);
            }
            *(s+i)=c;
        }
        s=realloc(s,sizeof(char)*(i+1));
        *(s+i)="\0";
        int id;
        for (int q = 0; q < 6; q++){
            char * token;
            if (token=strtok(s,";") != NULL && q < 2){
                switch (q){
                    case 0:
                        //leo el started_at
                        break;
                    case 1:
                        //leo el start_station_id
                        break;
                    case 2:
                        //leo el ended_at
                        break;
                    case 3:
                        //leo el end_station_id
                        break;
                    case 5:
                        //leo el member_casual: Si el usuario del alquiler es miembro del sistema de alquiler (cadena de caracteres, “casual” si no es miembro, “member” si lo es)
                        break;
                    default:
                        break;
                }
            }
        }
    }
    free(s);
    fclose(bikesNYC);
}