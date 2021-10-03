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

/**
 *  @brief Inicializace BVS 
 **/
void symTableInit(symTableNodePtr *tree){
    *tree = NULL;
}

/**
 *  @brief Hledání uzlu ve stromě podle identifikátoru
 **/
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
/**
 *  @brief Vložení nového uzlu do stromu
 **/
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

/**
 *  @brief Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu
 **/
void replaceByRightmost(symTableNodePtr replacedPtr, symTableNodePtr *tree){
    if(!(*tree)) return NULL;

    if((*tree)->rptr != NULL){
        replaceByRightmost(replacedPtr, &(*tree)->rptr);
    } else{
        replacedPtr->data = (*tree)->data;
        replacedPtr->key = (*tree)->key;
        symTableNodePtr tmp = (*tree);
        (*tree) = (*tree)->lptr;
        free(tmp);
    }
}

/**
 *  @brief Vymazání uzlu ze stromu
 **/
void symTableDelete(symTableNodePtr *tree, string key){
    if(!(*tree)) return NULL;

    if (strCmpString(&key, &(*tree)->key) < 0){
        symTableDelete(&(*tree)->lptr, key);
    }
    else if (strCmpString(&key, &(*tree)->key) > 0){
        symTableDelete(&(*tree)->rptr, key);
    }
    else{
        replaceByRightmost((*tree), (&(*tree)->lptr));
        if((*tree)->rptr == NULL)
            (*tree) = (*tree)->lptr;
        else
            (*tree) = (*tree)->rptr;
    }
}

/**
 *  @brief Zrušení celého stromu
 **/
void symTableDispose(symTableNodePtr *tree){
    if(!(*tree)) return NULL;

    symTableDispose(&(*tree)->lptr);
    symTableDispose(&(*tree)->rptr);
    free(*tree);
    *tree = NULL;
}