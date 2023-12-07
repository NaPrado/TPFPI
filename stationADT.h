#ifndef __stationADT_h_
#define __stationADT_h_
typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
typedef struct station * pStation ;
char isValidRental();

stationADT newStation(void);

void addStation(stationADT station,char * stationName, stationsIdBST idBst, size_t stationId);

void addRental(stationsIdBST idBST, struct tm * startDate,size_t startId,struct tm * endDate, size_t endId, char association);

//quiza no haga falta 
stationADT deleteStation();

void freeStation();

#endif