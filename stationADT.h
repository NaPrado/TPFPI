#ifndef __stationADT_h_
#define __stationADT_h_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
typedef struct stationCDT * stationADT; 
typedef struct stationsIdNode *stationsIdBST;
typedef struct station * pStation ;
typedef struct bst *bst;

/*
Para algunas funciones se necesita de un arbol auxiliar. Esta funcion lo inicializa.
*/
bst newtree(void);
/*
Se crea el ADT en donde se guardan los datos y se allocan todos los recursos que necesita.
*/
stationADT newStation(void);
/*
Se liberan los recursos del arbol. Recibe al arbol que se quiere liberar.
*/
void freeTree(bst root);
/*
Ordena segun un criterio util a la query 1 los recursos guardados en el ADT. Recibe al ADT que se quiera ordenar.
*/
void orderByCount(stationADT stations);
/*
Añade al ADT una estación. Recibe al ADT, el nombre de la estacion, a un arbol inicializado y la Id de la misma.
*/
void addStation(stationADT station,char * stationName, bst rootbst, size_t stationId);
/*
Añade un alquiler al ADT. Recibe al arbol, fecha de entrega, id de la estacion de entrega, fecha de devolución, 
id de la estación de devolución, tipo de asociasion (si es miembro o casual) y el ADT.
*/
void addRental(bst idBST, struct tm startDate,size_t startId,struct tm endDate, size_t endId, char association, stationADT station);
/*
Funcion que cumple los requisitos de la query 1. Estos son: devolver un archivo .csv y otro .html donde esten todas las estaciones con la 
cantidad de viajes totales y la cantidad de viajes segun asociacion. Se muestran en orden descendete por la cantidad total de viajes iniciados.
*/
void query1(stationADT stations);
/*
Funcion que cumple los requisitos de la query 2. Estos son: devolver un archivo .csv y otro .html donde en ambos esten las estaciones
y la fecha de su entrega de bicleta mas antiguo. En caso de no haber viajes ese dia se muestra 0. Las fechas se muestran en formato
DD/MM/YYYY HH:mm. Se muestran las estaciones en orden alfabetico.
*/
void query2(stationADT stations);
/*
Funcion que cumple los requisitos de la query 3. Estos son: devolver un archivo .csv y otro .html donde en ambos esten la cantidad total
de viajes inciados y finalizados por cada dia de la semana.
*/
void query3(stationADT stations);
/*
Libera todos los recursos empleados por el ADT. Recibe al ADT que se quiera liberar.
*/
void freeAssets(stationADT stations);

#endif