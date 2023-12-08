#ifndef __stationADT_h_
#define __stationADT_h_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
typedef struct station * pStation ;

char* copyString(const char *origen);

stationADT newStation(void);

void freeTree(stationsIdBST root);

void orderByCount(stationADT stations);

void addStation(stationADT station,char * stationName, stationsIdBST idBst, size_t stationId);

void addRental(stationsIdBST idBST, struct tm * startDate,size_t startId,struct tm * endDate, size_t endId, char association);

void query1(stationADT stations);

void query2(stationADT stations);

void query3(stationADT stations);

void freeAssets(stationADT stations);

#endif