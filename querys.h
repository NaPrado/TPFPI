#ifndef __querys_h_
#define __querys_h_
#include "stationADT.h"
/*
Funcion que cumple los requisitos de la query 1. Estos son: devolver un archivo .csv y otro .html donde esten todas las estaciones con la 
cantidad de viajes totales y la cantidad de viajes segun asociacion. Se muestran en orden descendete por la cantidad total de viajes iniciados.
*/
void query1(stationsADT stations);
/*
Funcion que cumple los requisitos de la query 2. Estos son: devolver un archivo .csv y otro .html donde en ambos esten las estaciones
y la fecha de su entrega de bicleta mas antiguo. En caso de no haber viajes ese dia se muestra 0. Las fechas se muestran en formato
DD/MM/YYYY HH:mm. Se muestran las estaciones en orden alfabetico.
*/
void query2(stationsADT stations);
/*
Funcion que cumple los requisitos de la query 3. Estos son: devolver un archivo .csv y otro .html donde en ambos esten la cantidad total
de viajes inciados y finalizados por cada dia de la semana.
*/
void query3(stationsADT stations);
/*
Funcion que cumple los requisitos de la query 4. Estos son: devolver un archivo .csv y otro .html donde en ambos esten la totalidad de las estaciones
listadas en orden alfabetico, cuales son las estaciones de fin más visitadas y cuantas veces. Ademas se debe tener en cuenta un rango de años.
*/
void query4(stationsADT stations);

#endif
