/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 * 
 * Soubor:
 *      @file symtable.c
 * 
 * Popis:
 *      @brief Tabulka symbolů implementovaná jako binární vyhledávací strom.
 */

#include "symtable.h"

void symTableInit(symTableNodePtr *tree){
    *tree = NULL;
}

symTableDataPtr symTableSearch(symTableNodePtr *tree, string key){
    if(!(*tree)) return NULL;

    else if (strCmpString(&key, &(*tree)->key) < 0)
        return symTableSearch((*tree)->lptr, key);
    
    else if (strCmpString(&key, &(*tree)->key) > 0)
        return symTableSearch((*tree)->rptr, key);
}


/**
 *  TODO!! nejspíš bude crashovat protože pointery a bude potřebovat možná i trochu domyslet --xhlins01
 */
void symTableInsert(symTableNodePtr *tree, string key, symTableDataPtr data){
    if(!(*tree)){
        symTableNodePtr temp = malloc(sizeof(struct symTableNode));
        if(temp == NULL) return;

        temp->data = data;
        temp->key = key;
        temp->lptr = NULL;
        temp->rptr = NULL;

        return;
    }

    if (strCmpString(&key, &(*tree)->key) < 0){
        symTableInsert((*tree)->lptr, key, data);
        return;
    }
    else if (strCmpString(&key, &(*tree)->key) > 0){
        symTableInsert((*tree)->rptr, key, data);
        return;
    } 
}