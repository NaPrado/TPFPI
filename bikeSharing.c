#include <stdio.h>
#include <stdlib.h>
#include "stationADT.h"
#include "read.h"
#include "querysADT.h"

int main(int argc, char const *argv[])
{
    if(argc < 3){
        printf("Cantidad de argumentos incorrectos\n");
        exit(EXIT_FAILURE);
    }
    if (argc > 5)
    {
        printf("Advetencia, la cantidad de argumentos es mayor a la solicitada\n");
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