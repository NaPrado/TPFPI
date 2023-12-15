#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stationADT.h"
#include "read.h"
#include "querysADT.h"

int main(int argc, char const *argv[])
{   
    time_t t=time(NULL);
    stationsADT new= newStationsGroup(argc,argv);
    readCSVFileStation(argv,new);
    query1(new);
    query2(new);
    query3(new);
    query4(new);
    freeAssets(new);
    printf("%ld\n",time(NULL)-t);
    return 0;
}