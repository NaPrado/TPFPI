#include "tree.h"
#include <stdio.h>

typedef struct node * pNode;

struct node{
    size_t id;
    void * elem;
    pNode left;
    pNode right;
};

struct bstCDT {
	pNode root;	    // raíz del arbol
};

bstADT newtree(void) {
    return calloc(1,sizeof(struct bstCDT));
}

static pNode addToTreeRec(pNode root, size_t id, void * elem){
    if (root == NULL){
        pNode aux =calloc(1,sizeof(struct node));
        aux->id=id;
        aux->elem=elem; 
        return aux;
    }
    int c=(root->id - id);
    if (c < 0){
        root->right = addToTreeRec(root->right,id,elem);
    }
    else if (c>0){
        root->left = addToTreeRec(root->left,id,elem);
    }
    return root;
}

void addToTree(bstADT bst, size_t id, void * elem){
    bst->root = addToTreeRec(bst->root,id,elem);
    return;
}

void * existIdRec(size_t id, pNode root){
    if(root == NULL){
        return NULL;
    }
    int c;
    if((c=root->id-id)>0){
        return isValidId(id,root->left);
    }
    if(c<0){
        return isValidId(id,root->right);
    }
    return root->elem;
}

void * existId(size_t id,bstADT tree){
    return existIdRec(id,tree->root);
}

static void freeTreeRec(pNode root){
    if(root == NULL){
        return;
    }
    freeTreeRec(root->left);
    freeTreeRec(root->right);
    free(root);
}

void freeTree(bstADT root){
    freeTreeRec(root->root);
    free(root);
}  