typedef struct stationCDT * stationADT; 

char isValidRental();

stationADT inicializerNYCFormat(char const argv[]);

stationADT inicializerMONFormat(char const argv[]);

stationADT newStation(void);

stationADT addStation();

//quiza no haga falta 
stationADT deleteStation();

void freeStation();