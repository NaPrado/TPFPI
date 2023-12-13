#ifndef __treeADT_h_
#define __treeADT_h_
#include <stdio.h>

typedef struct bstCDT * bstADT;

/* 
Este ADT permite el uso de ids asociadas a elementos para saber si existen y si existen donde estan guardadas
*/


/*
Funcion constructora de un arbol binario
*/
bstADT newtree(void);
/* 
Añadir una nueva hoja al arbol, esta contiene la id y su elemento asociado
*/
void addToTree(bstADT bst, size_t id, void * elem);
/* 
busca una id en el arbol y si existe retorna su elemento asociado
*/
void * existId(size_t id,bstADT tree);
/*
Se liberan los recursos del arbol. Recibe al arbol que se quiere liberar.
*/
void freeTree(bstADT root);

#endif