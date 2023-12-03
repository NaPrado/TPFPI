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

struct stationsIdBST{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    struct station * associatedStation;
    struct stationsIdBST * left;
    struct stationsIdBST * right;
};


char isValidRental(){

}

stationADT inicializerMONFormat(char const argv[]){
    char * stationsMON = fopen( argv[1], "r");
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

}

stationADT inicializerNYCFormat(char const argv[]){

}

stationADT newStation(void){

}

stationADT addStation(){

}

//quiza no haga falta 
stationADT deleteStation(){

}

void freeStation(){

}

int main(int argc, char const *argv[])
{
    //lectura station
    char * arch1 = fopen( argv[1], "r");
    if(arch1 == NULL){
        exit (1);
    }
    //loop hasta \n
    int fgetc(arch1);
    char * s;
    strtok(s,";");


    fclose(arch1);
    //lectura bikes
    char * arch2 = fopen( argv[0], "r");
    if(arch1 == NULL){
        exit (1);
    }
    fclose(arch1);
    return 0;
}

