#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <math.h>
//#include <stdbool.h>
#include "stationADT.h"

#define BLOQUECHARS 10
#define MEMBER 1
#define CASUAL 0

// el tipo de dato es el mismo tanto para stationsNYC como stationsMON, lo que cambia es como obtenemos esos datos;

/* 
    Info importante de cada archivo
    Para bikes:
        start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member
        2021-09-20 06:31:28;348;2021-09-20 07:02:22;332;1
    Para stations:
        pk;name;latitude;longitude
        327;Sanguinet / de Maisonneuve;45.513405;-73.562594
*/

// ---------------------------------IDEA-----------------------------------------
// proponemos que el cdt contenga dos cosas primero dos listas de estaciones (alfabetico, cantidad viajes iniciados)
// segundo un arbol de ids
// esto con el proposito de que primero se cargan las informaciones de las estaciones existentes del archivo stations
// a su vez se iran cargando en paralelo las ids existentes de cada estacion a un arbol binario de busqueda 
// segundo se va copiar los viajes en el nodo de cada estacion si las estaciones de salida y llegada son validas (osea estan en el arbol)

typedef struct rental * pRental ;
struct rental //datos del archivo Bike se guardaran en formato de lista ordenada por stationIdStart 
{
    // size_t stationIdStart; redundante con la lista de estaciones
    struct tm * dateStart;
    char * stationNameEnd;
    struct tm * dateEnd;
    //bool is_member;
    pRental tail;
};

typedef struct station * pStation ;

struct station //lista
{
    char * stationName;
    // long double latitude; ver si es necesario
    // long double longitude;
    // size_t id;
    pRental oldestRental; //lista
    size_t amountRentalsByMembers;  //contadores para q1
    size_t amountRentalsByCasuals;  //contadores para q1
    size_t totalAmountRentals;      //contadores para q1
    pStation tailAlpha;
    pStation tailCount;             //util para q1
};

struct stationCDT
{
    pStation firstAlpha;//lista estaciones orden alfabetico (osea digamos puntero a primer nodo)
    pStation firstCount;//lista oredenada segun cantidad de viajes iniciados en esa estacion
};                              //osea digamos soy nahue, es necesario esto ??????????????????????????????????????????????????????????????????????????????????????????
                                //si nahue pq hay q saber cual es el primer elem, de hecho, necesitas un firstCount aca para tu nueva forma de ordenar 

struct stationsIdNode{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    struct station * associatedStation;
    struct stationsIdBST * left;
    struct stationsIdBST * right;
};

typedef struct stationsIdNode * stationsIdBST;



char * intToStr(int num){
    char * str = malloc((int)((ceil(log10(num))+1)*sizeof(char)));
    sprintf(str, "%d", num);
    return str;
}


//esta funcion debera ser llamada para asignar los tiempos de entrada/salida en las structs correspondientes
struct tm * assignDate(char * date ) //yyyy-MM-dd HH:mm:ss
{
    struct tm * ret = calloc(1,sizeof(struct tm));//es necesario que sea dinamico??? creo q no
        for (int q = 0; q < 6; q++){
            char * token;
            if (token=strtok(date,"- :") != NULL){
                switch (q){
                    case 0:
                        //leo el yyyy
                        ret->tm_year=atoi(token)-1900;
                        break;
                    case 1:
                        //leo el MM
                        ret->tm_mon=atoi(token)-1;
                        break;
                    case 2:
                        //leo el dd
                        ret->tm_mday=atoi(token);
                        break;
                    case 3:
                        //leo el HH
                        ret->tm_hour=atoi(token);
                        break;
                    case 4:
                        //leo el mm
                        ret->tm_min=atoi(token);
                        break;
                    case 5:
                        //leo el ss
                        ret->tm_sec=atoi(token);
                        break;
                    default:
                        break;
                }
            }
        }
    return ret;
}



stationADT newStation(void){
    stationADT new = calloc(1,sizeof(struct stationCDT));
    return new;
}

static void addToTree(stationsIdBST root, size_t id, struct station * associatedStation){
    if (root == NULL){
        root = calloc(1,sizeof(struct stationsIdNode));
        root->stationId = id;
        root->associatedStation = associatedStation;
    }else{
        if (id <= root->stationId){
            addToTree(root->left, id, associatedStation);
        }else{
            addToTree(root->right, id, associatedStation);
        }
    }
}

pStation addStation(pStation alphaList,char * stationName, stationsIdBST idBst, size_t stationId){
    if(alphaList == NULL || strcasecmp(alphaList->stationName,stationName) < 0){//si llegue al final o era vacia o tengo que añadir añado
        //incorporacion a la lista
        pStation newNode = calloc(1,sizeof(struct station));
        newNode->stationName = stationName; //ver si anda.
        newNode->tailAlpha = alphaList;
        //incorporacion a el BST
        addToTree(idBst,stationId,newNode);
        return newNode;
    }
    if(strcasecmp(alphaList->stationName,stationName) == 0){
        //ya estaba???
        return alphaList;
    }
    alphaList->tailAlpha = addStation(alphaList->tailAlpha, stationName, idBst, stationId);
    return alphaList;
}

static char isValidId(size_t id, stationsIdBST root, pStation * correctStation){
    if(root == NULL){
        return 0;
    }
    else{
        if(root->stationId == id){
            *correctStation = root->associatedStation;
            return 1;
        }
        else{
            if(id < root->stationId){
                return isValidId(id,root->left, correctStation);
            }
            else{
                return isValidId(id,root->right, correctStation);
            }
        }
    }
}

static char isValidRental(size_t startStationId, size_t endStationId, stationsIdBST idBst, pStation * startStation, pStation * endStation){
    return (startStationId != endStationId && (isValidId(startStationId, idBst, startStation) && isValidId(endStationId, idBst, endStation)));;
}

pRental addRentalRec(pRental rentalList, struct tm * startDate, struct tm * endDate, char * endStationName){
    double cmp;
    if(rentalList == NULL || (cmp = difftime(mktime(startDate),mktime(rentalList->dateStart))) < 0){
        pRental newRental = malloc(sizeof(struct rental));
        newRental->dateEnd=endDate;
        newRental->dateStart=startDate;
        newRental->stationNameEnd=endStationName;
        newRental->tail=rentalList;
        return newRental;
    }
    rentalList->tail = addRentalRec(rentalList->tail, startDate, endDate, endStationName);
    return rentalList;
}

void addRental(stationsIdBST idBST, struct tm * startDate,size_t startId,struct tm * endDate, size_t endId, char association){
    pStation startStation, endStation;
    if(!isValidRental(startId, endId, idBST, &startStation, &endStation)){
        return;
    }
    startStation->totalAmountRentals += 1;
    char * endStationName = endStation->stationName; //ver si anda (quilombitos de strings)
    startStation->oldestRental = addRentalRec(startStation->oldestRental, startDate, endDate, endStationName);
    if(association == MEMBER){
        startStation->amountRentalsByMembers += 1;
    }
    if(association == CASUAL){
        startStation->amountRentalsByCasuals += 1;
    }
}

//quiza no haga falta 
stationADT deleteStation(){

}

void freeAssets(){

}

stationADT inicializerNYCFormat(char const argv[],stationADT newStation){
    stationsIdBST tree=NULL;
    FILE * stationsNYC = fopen( argv[1], "rt");
    if(errno != 0){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones de Nueva York\n");
        exit (1); //poner codigo de erno
    }
    readIndex(stationsNYC);
    char * s=NULL;
    while (!feof(stationsNYC)){
        s=realloc(s,sizeof(char)*BLOQUECHARS);
        int i=0;
        for (int j=0, c ; c=fgetc(stationsNYC) != '\n' ; i++){
            if (i%BLOQUECHARS==0)
            {
                j++;
                s=realloc(s,sizeof(char)*BLOQUECHARS*j);
            }
            *(s+i)=c;
        }
        s=realloc(s,sizeof(char)*(i+1));
        *(s+i)="\0";
        char * name;
        for (int q = 0; q < 4; q++){
            char * token=strtok(s,";");
            if (token != NULL && q!=1 && q!=2){
                switch (q){
                    case 0:
                        //leo el name
                            strcpy(name,token);
                        break;
                    case 3:
                        //leo el id
                            newStation->firstAlpha = addStation(newStation->firstAlpha,name,tree,atoi(token));
                        break;
                    default:
                        break;
                }
            }
        }
    }
    free(s);
    fclose(stationsNYC);
}

stationADT inicializerMONFormat(char const argv[],stationADT newStation){
    stationsIdBST tree=NULL;
    errno = 0;
    FILE * stationsMON = fopen( argv[1], "rt");
    if(errno != 0 || stationsMON==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de las estaciones de Montreal\n");
        exit (1);
    }
    readIndex(stationsMON);
    char * s=NULL;
    while (!feof(stationsMON)){
        s=realloc(s,sizeof(char)*BLOQUECHARS);
        int i=0;
        for (int j=0, c ; c=fgetc(stationsMON) != '\n' ; i++){
            if (i%BLOQUECHARS==0)
            {
                j++;
                s=realloc(s,sizeof(char)*BLOQUECHARS*j);
            }
            *(s+i)=c;
        }
        s=realloc(s,sizeof(char)*(i+1));
        *(s+i)="\0";
        int id;
        for (int q = 0; q < 4; q++){
            char * token=strtok(s,";");
            if (token != NULL && q < 2){
                switch (q){
                    case 0:
                        //leo el id
                            id=atoi(token);
                        break;
                    case 1:
                        //leo el name
                            newStation->firstAlpha = addStation(newStation->firstAlpha,token,tree,id);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    free(s);
    fclose(stationsMON);
}