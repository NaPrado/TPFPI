#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stationADT.h"
#include "read.h"

int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("cantidad de argumentos incorrectos\n");
        exit(EXIT_FAILURE);
    }
    stationADT new= newStation();
    inicializerNYCFormat(argv,new);
    query1(new);
    query2(new);
    query3(new);
    freeAssets(new);
    return 0;
}