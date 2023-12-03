#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
//#include <stdbool.h>
#include "stationADT.h"
// el tipo de dato es el mismo tanto para stationsNYC como stationsMON, lo que cambia es como obtenemos esos datos;

/* 
    Info importante de cada archivo
    Para bykes:
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
    struct tm dateStart;
    char * stationNameEnd;
    struct tm dateStart;
    struct tm dateEnd;
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
};

struct stationCDT
{
    struct station * stations; //lista estaciones orden alfabetico (osea digamos puntero a primer nodo)
};


//esta funcion debera se llamada para asignar los tiempos de entrada/salida en las structs correspondientes
struct tm * assignTime(/*la funcion deberia recibir tiempo de alguna forma, no se si como char * con todos los datos y luego fragmentarlos, como varios char * en partes o como varios enteros*/)
{
    struct tm * date;
    date->tm_hour/*  = hora */;
    //el resto de los campos asignados
    return date;
}

struct stationsIdNode{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    struct station * associatedStation;
    struct stationsIdBST * left;
    struct stationsIdBST * right;
};

typedef struct stationsIdNode * stationsIdBST;


char isValidRental(){

}

void readIndex(FILE * stations){
    while (fgetc(stations)!='\n'){
    }
    return;
}

stationADT inicializerMONFormat(char const argv[]){
    FILE * stationsMON = fopen( argv[1], "r");
    if(stationsMON == NULL){
        printf("No se pudo abrir el archivo\n");
        exit (1);//deberia ser un return errno
    }
    readIndex(stationsMON);
    char * s;
    while (!feof(stationsMON)){
        for (int i=0, c ; c=fgetc(stationsMON) != "\n" ; i++)
            *(s+i)=c;
        for (int i = 0; i < 4; i++){
            char * token;
            if (token=strtok(s,";") != NULL && i < 2){
                switch (i){
                    case 0:
                        //leo el id
                            token;
                        break;
                    case 1:
                        //leo el name
                            token;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    fclose(stationsMON);
}

stationADT inicializerNYCFormat(char const argv[]){
    FILE * stationsNYC = fopen( argv[1], "r");
    if(stationsNYC == NULL){
        printf("No se pudo abrir el archivo\n");
        exit (1);//deberia ser un return errno
    }
    readIndex(stationsNYC);
    char * s;
    while (!feof(stationsNYC)){
        for (int i=0, c ; c=fgetc(stationsNYC) != "\n" ; i++)
            *(s+i)=c;
        for (int i = 0; i < 4; i++){
            char * token;
            if (token=strtok(s,";") != NULL && (i == 0 || i==3)){
                switch (i){
                    case 3:
                        //leo el id
                            token;
                        break;
                    case 0:
                        //leo el name
                            token;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    fclose(stationsNYC);
}

stationADT newStation(void){

}

static int nameCompare(char * name1, char *  name2){
    int i,toReturn;
    i=toReturn=0;
    while(name1[i] != 0 || name2 != 0 && toReturn == 0){
        toReturn = toupper(name1[i]) - toupper(name2[i]);
    }
    return toReturn;
}

static void addToTree(stationsIdBST root, size_t id, struct station * associatedStation){
    if (root == NULL){
        root = calloc(sizeof(struct stationsIdNode));
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
    if(stationsList->stations == NULL || nameCompare(stationsList->stations->stationName,stationName) < 0){//si llegue al final o era vacia o tengo que añadir añado
        //incorporacion a la lista
        struct station * newNode = calloc(1,sizeof(struct station));
        newNode->stationName = stationName; //ver si anda
        newNode->tailAlpha = stationsList->stations;
        //incorporacion a el BST
        addToTree(idBst,stationId,newNode);
        return newNode;
    }
    if(nameCompare(stationsList->stations->stationName,stationName) == 0){
        //ya estaba???
        return stationsList;
    }
    stationsList->stations = addStation(stationsList->stations, stationName, idBst, stationId);
    return stationsList;
}


//quiza no haga falta 
stationADT deleteStation(){

}

void freeStation(){

}

int main(int argc, char const *argv[])
{

    return 0;
}

