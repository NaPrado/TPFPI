#include <stdio.h>
#include <stdlib.h>
#include "stationADT.h"
#include "read.h"
#include <time.h>

int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("cantidad de argumentos incorrectos\n");
        exit(EXIT_FAILURE);
    }
    time_t t=time(NULL);
    stationADT new = newStation();
    inicializerMONFormat(argv,new);
    query1(new);
    query2(new);
    query3(new);
    freeAssets(new);
    printf("%ld\n",time(NULL)-t);
    return 0;
}
