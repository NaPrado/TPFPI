#include <stdio.h>
#include <stdlib.h>
#include "stationADT.h"
#include "read.h"
#include "querysADT.h"

int main(int argc, char const *argv[])
{
    printf("%d",argc);
    if(argc < 3){
        printf("cantidad de argumentos incorrectos\n");
        exit(EXIT_FAILURE);
    }
    stationsADT new= newStationsGroup(argc,argv);
    readCSVFileStation(argv,new);
    query1(new);
    query2(new);
    query3(new);
    query4(new);
    // query5(new); 
    freeAssets(new); 
    return 0;
}