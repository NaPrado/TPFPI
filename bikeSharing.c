#include <stdio.h>
#include <stdlib.h>
#include "stationADT.h"
#include "read.h"
#include "querys.h"

int main(int argc, char const *argv[])
{   
    stationsADT new= newStationsGroup(argc,argv);
    readCSVFileStation(argv,new);
    query1(new);
    query2(new);
    query3(new);
    query4(new);
    freeAssets(new);
    return 0;
}