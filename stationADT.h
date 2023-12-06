#ifndef __stationADT_h_
#define __stationADT_h_
typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
typedef struct station * pStation ;
char isValidRental();

void inicializerNYCFormat(char const *argv[],stationADT newStation);

void inicializerMONFormat(char const *argv[],stationADT newStation);

stationADT newStation(void);

pStation addStation(pStation alphaList,char * stationName, stationsIdBST idBst, size_t stationId);

//quiza no haga falta 
stationADT deleteStation();

void freeStation();

//te retorna un puntero a struct con los datos de fechas guardados, luego podemos usar mktime()
struct tm * assignDate(char * date ); //yyyy-MM-dd HH:mm:ss

char * intToStr(int num);
#endif