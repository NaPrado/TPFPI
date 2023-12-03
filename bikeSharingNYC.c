#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#define BLOQUECHARS 10
#include "stationADT.h"

stationADT inicializerMONFormat(char const argv[]){
    FILE * stationsMON = fopen( argv[1], "r");
    if(stationsMON == NULL){
        printf("No se pudo abrir el archivo\n");
        exit (1);//deberia ser un return errno
    }
    readIndex(stationsMON);
    while (!feof(stationsMON)){
        char * s=NULL;
        s=realloc(s,sizeof(char)*BLOQUECHARS);
        int i=0;
        for (int j=0, c ; c=fgetc(stationsMON) != "\n" ; i++){
            if (i%BLOQUECHARS==0)
            {
                j++;
                s=realloc(s,sizeof(char)*BLOQUECHARS*j);
            }
            *(s+i)=c;
        }
        *(s+i)="\0";
        for (int q = 0; q < 4; i+=3){
            char * token;
            if (token=strtok(s,";") != NULL && i < 2){
                switch (i){
                    case 3:
                        //leo el id
                            token;
                        break;
                    case 0:
                        //leo el name
                            token;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    fclose(stationsMON);
}