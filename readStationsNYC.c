#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#define BLOQUECHARS 10
#include "stationADT.h"

stationsIdBST tree=NULL;

stationADT inicializerMONFormat(char const argv[],stationADT newStation){
    FILE * stationsNYC = fopen( argv[1], "r");
    if(errno != 0){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones de Nueva York\n");
        exit (1);
    }
    readIndex(stationsNYC);
    char * s=NULL;
    while (!feof(stationsNYC)){
        s=realloc(s,sizeof(char)*BLOQUECHARS);
        int i=0;
        for (int j=0, c ; c=fgetc(stationsNYC) != "\n" ; i++){
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
        for (int q = 0; q < 4; q+=3){
            char * token;
            if (token=strtok(s,";") != NULL && q < 2){
                switch (q){
                    case 0:
                        //leo el name
                            id=atoi(token);
                        break;
                    case 3:
                        //leo el id
                            newStation=addStation(newStation,token,tree,id);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    free(s);
    fclose(stationsNYC);
}