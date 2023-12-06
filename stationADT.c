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

__ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        // Si no hay un búfer asignado o el tamaño es 0, asignar un nuevo búfer
        *n = 10; // Puedes ajustar el tamaño inicial según tus necesidades
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Error de asignación de memoria
        }
    }

    size_t pos = 0;
    int c;

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        (*lineptr)[pos++] = c;

        // Verificar si necesitamos redimensionar el búfer
        if (pos >= *n - 1) {
            *n *= 2; // Duplicar el tamaño del búfer
            char *new_lineptr = (char *)realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                free(*lineptr);
                return -1; // Error de realocación de memoria
            }
            *lineptr = new_lineptr;
        }
    }

    if (pos == 0 && c == EOF) {
        //free(*lineptr);
        return -1; // No se leyó ninguna línea y se alcanzó el final del archivo
    }

    (*lineptr)[pos] = '\0'; // Agregar el carácter nulo al final de la línea
    return pos; // Devolver el número de caracteres leídos
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


void inicializerBikesMONFormat(char const *argv[],stationADT newStation){
    errno=0;
    printf("%s",argv[1]);
    FILE * bikesMON = fopen( argv[1], "rt");
    if(errno != 0 && bikesMON==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Montreal\n");
        exit (1);
    }
    
    char * s = NULL;
    size_t longitud = 0;
    // Leer líneas desde el archivo
    errno=0;
    if(getline(&s, &longitud, bikesMON)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de viajes realizados en Montreal\n");
        exit (1);
    }
    printf("%s",s);
/*     char * format="start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member\n";
    if (strcmp(s,format)!=0){
        printf("Formato incorrecto, su formato es:\n%s\n y el correcto es:\n%s",s,format);
        exit(1);
    } */
    free(s);
    while (!feof(bikesMON)){
        s=NULL;
        /* if( */getline(&s, &longitud, bikesMON);/* ==-1){ */
/*         errno=0;
        perror("Ocurrio un error leyendo alguna la linea del archivo de viajes realizados en Montreal\n");
        exit (1);
        } */
    struct tm startDate, endDate;
    int idStart, idEnd, isMember;
            // Formato: yyyy-mm-dd HH:mm:ss;idStart;yyyy-mm-dd HH:mm:ss;idEnd;isMember
    int result = sscanf(s, "%d-%d-%d %d:%d:%d;%d;%d-%d-%d %d:%d:%d;%d;%d",
                        &startDate.tm_year, &startDate.tm_mon, &startDate.tm_mday,
                        &startDate.tm_hour, &startDate.tm_min, &startDate.tm_sec,
                        &idStart,
                        &endDate.tm_year, &endDate.tm_mon, &endDate.tm_mday,
                        &endDate.tm_hour, &endDate.tm_min, &endDate.tm_sec,
                        &idEnd,
                        &isMember);

        if (result == 15) {
            // La cadena se analizó correctamente, los valores están en las variables correspondientes.
            printf("%d-%02d-%02d %02d:%02d:%02d;\t", startDate.tm_year, startDate.tm_mon, startDate.tm_mday,
                   startDate.tm_hour, startDate.tm_min, startDate.tm_sec);
            printf("%d;\t", idStart);
            printf("%d-%02d-%02d %02d:%02d:%02d;\t", endDate.tm_year, endDate.tm_mon, endDate.tm_mday,
                   endDate.tm_hour, endDate.tm_min, endDate.tm_sec);
            printf("%d;\t", idEnd);
            printf("%d;\n", isMember);
        } 
        else if (result==0)
        {
            //goddddd
        }
        else {
            // Hubo un problema al analizar la cadena
            printf("Error al analizar la cadena\n");
        }
        free(s);
    }
    fclose(bikesMON);
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


void inicializerBikesMONFormat(char const *argv[],stationADT newStation){
    errno=0;
    FILE * bikesNYC = fopen( argv[1], "rt");
    if(errno != 0 && bikesNYC==NULL){
        perror("Ocurrio un error mientrar se abria el archivo de viajes realizados en Montreal\n");
        exit (1);
    }
    
    char * s = NULL;
    size_t longitud = 0;
    errno=0;
    if(getline(&s, &longitud, bikesNYC)==-1){
        perror("Ocurrio un error leyendo la primer linea del archivo de viajes realizados en Nueva York\n");
        exit (1);
    }
    free(s);
    while (!feof(bikesNYC)){
        s=NULL;
        getline(&s, &longitud, bikesNYC);
        struct tm startDate, endDate;
        int idStart, idEnd;
        char isMember;
                // Formato: yyyy-mm-dd HH:mm:ss;idStart;yyyy-mm-dd HH:mm:ss;idEnd;rideable_type;member_casual
        int result = sscanf(s, "%d-%d-%d %d:%d:%d.000000;%d;%d-%d-%d %d:%d:%d.000000;%d;%*[^;];%c\n",
                            &startDate.tm_year, &startDate.tm_mon, &startDate.tm_mday, &startDate.tm_hour, &startDate.tm_min, &startDate.tm_sec,
                            &idStart,
                            &endDate.tm_year, &endDate.tm_mon, &endDate.tm_mday,&endDate.tm_hour, &endDate.tm_min, &endDate.tm_sec,
                            &idEnd,&isMember);
        if (result == 15) {
            // La cadena se analizó correctamente, los valores están en las variables correspondientes.
            printf("%d-%02d-%02d %02d:%02d:%02d;\t", startDate.tm_year, startDate.tm_mon, startDate.tm_mday,
                   startDate.tm_hour, startDate.tm_min, startDate.tm_sec);
            printf("%d;\t", idStart);
            printf("%d-%02d-%02d %02d:%02d:%02d;\t", endDate.tm_year, endDate.tm_mon, endDate.tm_mday,
                   endDate.tm_hour, endDate.tm_min, endDate.tm_sec);
            printf("%d;\t", idEnd);
            printf("%d;\n", isMember=='m');
        } else {
            // Hubo un problema al analizar la cadena
            printf("Error al analizar la cadena\n");
        }
            free(s);
    }
    fclose(bikesNYC);
}

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