#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
//#include <stdbool.h>
#include "stationADT.h"
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
// proponemos que el cdt contenga dos cosas primero una lista de estaciones
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
    pStation tailcount;             //util para q1
};

struct stationCDT
{
    struct station * stations;  //lista estaciones orden alfabetico (osea digamos puntero a primer nodo)
};                              //osea digamos soy nahue, es necesario esto ??????????????????????????????????????????????????????????????????????????????????????????


//esta funcion debera ser llamada para asignar los tiempos de entrada/salida en las structs correspondientes
struct tm * assignDate(char * date ) //yyyy-MM-dd HH:mm:ss
{
    struct tm * ret = calloc(1,sizeof(struct tm));//es necesario que sea dinamico??? creo q no
        for (int q = 0; q < 6; q++){
            char * token;
            if (token=strtok(date,"- :") != NULL && q < 2){
                switch (q){
                    case 0:
                        //leo el yyyy
                        ret->tm_year=atoi(token); // No me queda claro si hay que restar 1900 al año, revisar la documentacion 
                        break;
                    case 1:
                        //leo el MM
                        ret->tm_mon=atoi(token);
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

struct stationsIdNode{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    struct station * associatedStation;
    struct stationsIdBST * left;
    struct stationsIdBST * right;
};

typedef struct stationsIdNode * stationsIdBST;

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

stationADT addStation(stationADT stationsList,char * stationName, stationsIdBST idBst, size_t stationId){
    if(stationsList->stations == NULL || strcasecmp(stationsList->stations->stationName,stationName) < 0){//si llegue al final o era vacia o tengo que añadir añado
        //incorporacion a la lista
        struct station * newNode = calloc(1,sizeof(struct station));
        newNode->stationName = stationName; //ver si anda
        newNode->tailAlpha = stationsList->stations;
        //incorporacion a el BST
        addToTree(idBst,stationId,newNode);
        return newNode;
    }
    if(strcasecmp(stationsList->stations->stationName,stationName) == 0){
        //ya estaba???
        return stationsList;
    }
    stationsList->stations = addStation(stationsList->stations, stationName, idBst, stationId);
    return stationsList;
}

static char isValidId(size_t id, stationsIdBST root){
    if(root == NULL){
        return 0;
    }
    else{
        if(root->stationId == id){
            return 1;
        }
        else{
            if(id < root->stationId){
                return isValidId(id,root->left);
            }
            else{
                return isValidId(id,root->right);
            }
        }
    }
}

static char isValidRental(size_t startStationId, size_t endStationId, stationsIdBST idBst){
    return (isValidId(startStationId, idBst) && isValidId(endStationId, idBst));
}

pRental addRental(){

}

//quiza no haga falta 
stationADT deleteStation(){

}

void freeAssets(){

}

int main(int argc, char const *argv[])
{

    return 0;
}