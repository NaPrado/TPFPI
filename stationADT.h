typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
char isValidRental();

stationADT inicializerNYCFormat(char const argv[],stationADT newStation);

stationADT inicializerMONFormat(char const argv[],stationADT newStation);

stationADT newStation(void);

stationADT addStation(stationADT stationsList,char * stationName, stationsIdBST idBst, size_t stationId);

//quiza no haga falta 
stationADT deleteStation();

void freeStation();