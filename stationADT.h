#ifndef __stationADT_h_
#define __stationADT_h_
#include <stdlib.h>
#include <stdio.h>
typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
typedef struct station * pStation ;
typedef struct bst *bst;

bst newtree(void);

char* copyString(const char *origen);

stationADT newStation(void);

void freeTree(bst root);

void orderByCount(stationADT stations);

void addStation(stationADT station,char * stationName, bst rootbst, size_t stationId);

void addRental(bst idBST, struct tm * startDate,size_t startId,struct tm * endDate, size_t endId, char association);

void query1(stationADT stations);

void query2(stationADT stations);

void query3(stationADT stations);

void freeAssets(stationADT stations);

#endif