#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stationADT.h"
#include "read.h"

int main(int argc, char const *argv[])
{
    time_t t=time(NULL);
    if(argc != 2){
        printf("cantidad de argumentos incorrectos\n");
        exit(EXIT_FAILURE);
    }
    stationADT new = newStation();
    inicializerMONFormat(argv,new);
    query1(new);
    query2(new);
    query3(new);
    freeAssets(new);
    printf("%ld\n",time(NULL)-t);
    return 0;
}
