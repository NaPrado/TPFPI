#ifndef __stationADT_h_
#define __stationADT_h_
#include <stdlib.h>
#include <stdio.h>
typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
typedef struct station * pStation ;

char* copiarCadena(const char *origen);

stationADT newStation(void);

void addStation(stationADT station,char * stationName, stationsIdBST idBst, size_t stationId);

void addRental(stationsIdBST idBST, struct tm * startDate,size_t startId,struct tm * endDate, size_t endId, char association);

//quiza no haga falta 
stationADT deleteStation();

void query1(stationADT stations);

void query2(stationADT stations);

void query3(stationADT stations);

void freeStation(stationADT stations);

void freeAssets();

#endif