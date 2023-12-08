#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stationADT.h"
#include "readMON.h"

int main(int argc, char const *argv[])
{
    time_t t=time(NULL);
    stationADT new= newStation();
    inicializerMONFormat(argv,new);
    query1(new);
    query2(new);
    query3(new);
    //freeAssets();
    printf("%ld\n",time(NULL)-t);
    return 0;
}
