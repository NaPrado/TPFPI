#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <math.h>
#include "libreria HTML/htmlTable.h"  
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

/* typedef struct station * pStation ; */

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
    pStation firstAlpha;//lista estaciones orden alfabetico (puntero a primer nodo)
    pStation firstCount;//lista oredenada segun cantidad de viajes iniciados en esa estacion
};                              

struct stationsIdNode{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    struct station * associatedStation;
    struct stationsIdBST * left;
    struct stationsIdBST * right;
};

typedef struct stationsIdNode * stationsIdBST;

char* copiarCadena(const char *origen) {
    // Obtener la longitud de la cadena de origen
    size_t longitud = strlen(origen);

    // Asignar memoria dinámica para la cadena de destino
    char *destino = (char*)malloc((longitud + 1) * sizeof(char));

    // Verificar si la asignación de memoria fue exitosa
    if (destino == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    // Copiar la cadena de origen a la zona de memoria dinámica
    strcpy(destino, origen);

    // Devolver la dirección de la zona de memoria asignada
    return destino;
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

void addStation(stationADT station,char * stationName, stationsIdBST idBst, size_t stationId){
    char * name =copiarCadena(stationName);
    station->firstAlpha=addStationRec(station->firstAlpha,name,idBst,stationId);
    return;
}

static pStation addStationRec(pStation alphaList,char * stationName, stationsIdBST idBst, size_t stationId){
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
    alphaList->tailAlpha = addStationRec(alphaList->tailAlpha, stationName, idBst, stationId);
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
    return (isValidId(startStationId, idBst, startStation) && isValidId(endStationId, idBst, endStation));;
}

static pRental addRentalRec(pRental rentalList, struct tm * startDate, struct tm * endDate, char * endStationName){
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
    // no olvidar hacer free de nombres
}


/* int main(int argc, char const *argv[])
{
    time_t t=time(NULL);
    stationADT newStation;
    newStation=calloc(1,sizeof(stationADT));
    inicializerBikesMONFormat(argv,newStation);
    free(newStation);
    printf("%ld\n",time(NULL)-t);
    return 0;
} */



/* int main(int argc, char const *argv[])
{
    time_t t=time(NULL);
    //stationADT newStation;
    //newStation=calloc(1,sizeof(stationADT));
    inicializerBikesMONFormat(argv,newStation);
    //free(newStation);
    printf("%ld\n",time(NULL)-t);
    return 0;
}
 */


















static pStation link(pStation station,pStation listCount){
    if(listCount == NULL || station->totalAmountRentals <= listCount->totalAmountRentals){
        station->tailCount = listCount;
        listCount->tailCount = station;
        return listCount;
    }
    listCount->tailCount = link(station,listCount->tailCount);
    return listCount;
}

static void orderByCount(stationADT stations){
    pStation aux = stations->firstAlpha;
    while (aux != NULL){
        stations->firstCount = link(aux,stations->firstCount);
        aux = aux->tailAlpha;
    }
    return;
}

static void writeQ1Rec(pStation stations, htmlTable tablaQ1, FILE * csvQ1){
    if (stations==NULL)
        return;
    addHTMLRow(tablaQ1,stations->stationName,stations->amountRentalsByMembers,stations->amountRentalsByCasuals,stations->totalAmountRentals);
    fprintf(csvQ1,"%s;%d;%d;%d\n",stations->stationName,stations->amountRentalsByMembers,stations->amountRentalsByCasuals,stations->totalAmountRentals);
    writeQ1Rec(stations->tailCount,tablaQ1,csvQ1);
    return;
}

static void writeQ1(stationADT stations){
    errno = 0;
    FILE * csvQ1 = fopen("query1.csv","wt");
    if(errno != 0 || csvQ1==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fprintf(csvQ1,"bikeStation;memberTrips;casualTrips;allTrips\n");
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    writeQ1Rec(stations->firstCount,tablaQ1,csvQ1);//funcion recursva o iterativa que carga tanto html como csv
    closeHTMLTable(tablaQ1);
    fclose(csvQ1);
}


void query1(stationADT stations){
    orderByCount(stations);
    writeQ1(stations);//carga tanto html como csv
};











static pRental checkIfCircular(pRental rent, char * currentStationName){
    if (strcmp(currentStationName,rent->stationNameEnd)==0){
        return rent;
    }
    checkIfCircular(rent->tail,currentStationName);
}




static void writeQ2Rec(pStation stations, htmlTable tablaQ2, FILE * csvQ2){
    if (stations==NULL)
        return;
    pRental rent = checkIfCircular(stations->oldestRental,stations->stationName);
    char * s;
    sscanf(s,"%d/%d/%d %d:%d",rent->dateStart->tm_mday,rent->dateStart->tm_mon,rent->dateStart->tm_year,
    rent->dateStart->tm_hour,rent->dateStart->tm_min);
    addHTMLRow(tablaQ2,stations->stationName,rent->stationNameEnd,s);
    fprintf(csvQ2,"%s;%s;%d/%d/%d %d:%d\n",
    stations->stationName,
    rent->stationNameEnd,
    rent->dateStart->tm_mday,rent->dateStart->tm_mon,rent->dateStart->tm_year,
    rent->dateStart->tm_hour,rent->dateStart->tm_min);
    writeQ2Rec(stations->tailAlpha,tablaQ2,csvQ2);
    return;
}
//DD/MM/YYYY HH:mm
static void query2(struct stationCDT * stations){
    errno = 0;
    FILE * csvQ2 = fopen("query2.csv","wt");
    if(errno != 0 || csvQ2==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fputs("bikeStation;bikeEndStation;oldestDateTime\n",csvQ2);
    htmlTable tablaQ2 = newTable("query2.html",3,"bikeStation","bikeEndStation","oldestDateTime");
    writeQ2Rec(stations->firstAlpha,tablaQ2,csvQ2);//funcion recursva o iterativa que carga tanto html como csv
    closeHTMLTable(tablaQ2);
    fclose(csvQ2);
}
