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
#define DAYS_IN_WEEK 7



enum DAYS           
{
    saturday,     
    sunday,     
    monday,       
    tuesday,
    wednesday,     
    thursday,
    friday
} workday;




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
    struct tm * dateStart;
    char * stationNameEnd;
    struct tm * dateEnd;
    pRental tail;
};

struct station //lista
{
    char * stationName;
    pRental oldestRental; //lista
    size_t amountRentalsByMembers;  //contadores para q1
    size_t amountRentalsByCasuals;  //contadores para q1
    size_t totalAmountRentals;      //contadores para q1
    struct station * tailAlpha;
    struct station * tailCount;             //util para q1
};

struct stationCDT
{
    pStation firstAlpha;//lista estaciones orden alfabetico (puntero a primer nodo)
    pStation firstCount;//lista oredenada segun cantidad de viajes iniciados en esa estacion
};                              
struct stationsIdNode{ //arbol binario de busqueda basado en cada id de estacion
    size_t stationId;
    pStation associatedStation;
    struct stationsIdNode *  left;
    struct stationsIdNode * right;
};

struct bst {
	stationsIdBST root;	    // raíz del arbol
};

char* copyString(const char * origin) {
    // Obtener la longitud de la cadena de origen
    size_t length = strlen(origin);

    // Asignar memoria dinámica para la cadena de destino
    char *toReturn = (char*)malloc((length + 1) * sizeof(char));

    // Verificar si la asignación de memoria fue exitosa
    if (toReturn == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    // Copiar la cadena de origen a la zona de memoria dinámica
    strcpy(toReturn, origin);

    // Devolver la dirección de la zona de memoria asignada
    return toReturn;
}

bst newtree(void) {
    return calloc(1,sizeof(struct bst));
}

stationADT newStation(void){
    return calloc(1,sizeof(struct stationCDT));
}




static stationsIdBST addToTreeRec(stationsIdBST root, size_t id, struct station * associatedStation){
    if (root == NULL){
        stationsIdBST aux =calloc(1,sizeof(struct stationsIdNode));
        aux->stationId=id;
        aux->associatedStation=associatedStation;
        return aux;
    }
    int c=(root->stationId - id);
    if (c < 0)
    {
        root->right=addToTreeRec(root->right,id,associatedStation);
    }
    else if (c>0)
    {
        root->left =addToTreeRec(root->left,id,associatedStation);
    }
    return root;
}

static void addToTree(bst bst, size_t id, struct station * associatedStation){
    bst->root = addToTreeRec(bst->root,id,associatedStation);
    return;
}

static pStation addStationRec(pStation alphaList,char * stationName, bst rootbst, size_t stationId){
    int c;
    if(alphaList == NULL ||  (c=strcasecmp(alphaList->stationName, stationName)) > 0){//si llegue al final o era vacia o tengo que añadir añado
        //incorporacion a la lista
        pStation newNode = calloc(1,sizeof(struct station));
        newNode->stationName = stationName; //ver si anda.
        newNode->tailAlpha =alphaList;
        //incorporacion a el BST
        addToTree(rootbst,stationId,newNode);
        return newNode;
    }
    if(c<0)
        alphaList->tailAlpha=addStationRec(alphaList->tailAlpha,stationName,rootbst,stationId);
    return alphaList;
}

void addStation(stationADT station,char * stationName, bst rootbst, size_t stationId){
    char * name =copyString(stationName);
    station->firstAlpha=addStationRec(station->firstAlpha,name,rootbst,stationId);
    return;
}

static char isValidIdRec(size_t id, stationsIdBST root, pStation * correctStation){
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
                return isValidIdRec(id,root->left, correctStation);
            }
            else{
                return isValidIdRec(id,root->right, correctStation);
            }
        }
    }
}

static char isValidId(size_t id, bst root, pStation * correctStation){
    return isValidIdRec(id,root->root,correctStation);
}

static char isValidRental(size_t startStationId, size_t endStationId, bst idBst, pStation * startStation, pStation * endStation){
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

void addRental(bst idBST, struct tm * startDate,size_t startId,struct tm * endDate, size_t endId, char association){
    pStation startStation, endStation;
    if(!isValidRental(startId, endId, idBST, &startStation, &endStation)){
        return;
    }
    startStation->totalAmountRentals += 1;
    startStation->oldestRental = addRentalRec(startStation->oldestRental, startDate, endDate, endStation->stationName);
    if(association == MEMBER){
        startStation->amountRentalsByMembers += 1;
    }
    if(association == CASUAL){
        startStation->amountRentalsByCasuals += 1;
    }
}

void freeTreeRec(stationsIdBST root){
    if(root == NULL){
        return;
    }
    freeTreeRec(root->left);
    freeTreeRec(root->right);
    free(root);
}

void freeTree(bst root){
    freeTreeRec(root->root);
    free(root);
}   

static void freeRentals(pRental rentalList){
    if(rentalList == NULL){
        return;
    }
    free(rentalList->dateEnd);
    free(rentalList->dateStart);
//    free(rentalList->stationNameEnd);
    freeRentals(rentalList->tail);
    free(rentalList);
    return;
}

static void freeStations(pStation stationList){
    if(stationList == NULL){
        return;
    }
    freeStations(stationList->tailAlpha);
    free(stationList->stationName);
    freeRentals(stationList->oldestRental);
    free(stationList);
    return;
}

void freeAssets(stationADT stations){
    //lista de cosas a liberar: el adt,cada estacion,cada rental,cada nombre
    freeStations(stations->firstAlpha);
    free(stations);
}


static pStation link(pStation listAlpha,pStation listCount){
    if(listCount == NULL || listAlpha->totalAmountRentals >= listCount->totalAmountRentals){
        listAlpha->tailCount = listCount;
        return listAlpha;
    }
    listCount->tailCount = link(listAlpha,listCount->tailCount);
    return listCount;
}

void orderByCount(stationADT stations){
    pStation aux = stations->firstAlpha;
    while (aux != NULL){
        stations->firstCount = link(aux,stations->firstCount);
        aux = aux->tailAlpha;
    }
    return;
}

int countDigit(int num) {
    int count = 0;

    // Manejar caso especial para el número 0
    if (num == 0) {
        return 1;
    }

    // Contar los dígitos del número
    while (num != 0) {
        num /= 10;
        count++;
    }

    return count;
}

static void writeQ1Rec(pStation stations, htmlTable tablaQ1, FILE * csvQ1){
    if (stations==NULL)
        return;
    char * members=calloc(1,sizeof(char)*(countDigit(stations->amountRentalsByMembers)+1));
    char *casuals=calloc(1,sizeof(char)*(countDigit(stations->amountRentalsByCasuals)+1));
    char *total=calloc(1,sizeof(char)*(countDigit(stations->totalAmountRentals)+1));
    
    sprintf(members,"%zu",stations->amountRentalsByMembers);
    sprintf(casuals,"%zu",stations->amountRentalsByCasuals);
    sprintf(total,"%zu",stations->totalAmountRentals);
    addHTMLRow(tablaQ1,stations->stationName,members,casuals,total);
    fprintf(csvQ1,"%s;%ld;%ld;%ld\n",stations->stationName,stations->amountRentalsByMembers,stations->amountRentalsByCasuals,stations->totalAmountRentals);
    free(members);
    free(casuals);
    free(total);
    writeQ1Rec(stations->tailCount,tablaQ1,csvQ1);
    return;
}

void query1(stationADT stations){
    errno = 0;
    FILE * csvQ1 = fopen("query1.csv","wt");
    if(errno != 0 || csvQ1==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fprintf(csvQ1,"bikeStation;memberTrips;casualTrips;allTrips\n");
    htmlTable tablaQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
    writeQ1Rec(stations->firstAlpha,tablaQ1,csvQ1);//funcion recursva o iterativa que carga tanto html como csv
    closeHTMLTable(tablaQ1);
    fclose(csvQ1);
}


static pRental checkIfCircular(pRental rent, char * currentStationName){
    if (strcmp(currentStationName,rent->stationNameEnd)==0){
        return rent;
    }
    return checkIfCircular(rent->tail,currentStationName);
}

static void writeQ2Rec(pStation stations, htmlTable tablaQ2, FILE * csvQ2){
    if (stations==NULL)
        return;
    pRental rent = checkIfCircular(stations->oldestRental,stations->stationName);
    char * s=NULL;
    sprintf(s,"%d/%d/%d %d:%d",rent->dateStart->tm_mday,rent->dateStart->tm_mon,rent->dateStart->tm_year,
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
void query2(struct stationCDT * stations){
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

static int dayOfWeek(int day, int month, int year){
    if (month < 3) {
        month += 12;
        year--;
    }
    int yearInCentury = year % 100;
    int century = year / 100 ;
    int h = ( day + (13 * ( month + 1 )) / 5 + yearInCentury + yearInCentury / 4 + century / 4 - 2 * century ) % 7;
    return h;
}

static void countTrips(pRental rentalList, size_t * startedTrips, size_t * endedTrips){
    while(rentalList != NULL){
        startedTrips[dayOfWeek(rentalList->dateStart->tm_mday,rentalList->dateStart->tm_mon+1,rentalList->dateStart->tm_year+1900)]+=1;
        endedTrips[dayOfWeek(rentalList->dateEnd->tm_mday,rentalList->dateEnd->tm_mon+1,rentalList->dateEnd->tm_year+1900)]+=1;
        rentalList=rentalList->tail;
    }
    return;
}

static void writeQ3(size_t * startedTrips, size_t * endedTrips){
    char * weekDays[]={"Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday"};
    errno = 0;
    FILE * csvQ3 = fopen("query3.csv","wt");
    if(errno != 0 || csvQ3==NULL){
        perror("Ocurrio un error mientrar se creaba el archivo \"query1.csv\" \n");
        exit (1);
    }
    fputs("weekDay;startedTrips;endedTrips\n",csvQ3);
    htmlTable tablaQ3 = newTable("query3.html",3,"weekDay","startedTrips","endedTrips");
    for (size_t i = monday, j=i; i < DAYS_IN_WEEK+monday; i++)
    {
        if (i%friday==0)
        {
            j=saturday;
        }
        else{
            j++;
        }
        fscanf(csvQ3,"%s;%lu;%lu\n",*(weekDays+j),startedTrips+j,endedTrips+j);
        char * sT=NULL, * eT=NULL;
        sprintf(sT,"%ld",startedTrips[j]);
        sprintf(eT,"%ld",endedTrips[j]);
        addHTMLRow(tablaQ3,*(weekDays+j),sT,eT);
    }
    closeHTMLTable(tablaQ3);
    fclose(csvQ3);
}

void query3(stationADT stations){
    size_t startedTrips[DAYS_IN_WEEK];
    size_t endedTrips[DAYS_IN_WEEK];
    pStation aux = stations->firstCount;
    while(aux != NULL && aux->totalAmountRentals != 0){
        countTrips(aux->oldestRental, startedTrips, endedTrips);
        aux=aux->tailCount;
    }
    writeQ3(startedTrips,endedTrips);
}
