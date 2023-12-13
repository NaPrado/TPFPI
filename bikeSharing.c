#include <stdio.h>
#include <stdlib.h>
#include "stationADT.h"
#include "read.h"
#include "querysADT.h"

int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("cantidad de argumentos incorrectos\n");
        exit(EXIT_FAILURE);
    }
    stationsADT new= newStationsGroup(1,2);
    readCSVFileStation(argv,new);
    query1(new);
    query2(new);
    query3(new);
    freeAssets(new);
    return 0;
}