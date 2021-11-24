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
        return symTableSearch(&(*tree)->lptr, key);
    
    else if (strCmpString(&key, &(*tree)->key) > 0)
        return symTableSearch(&(*tree)->rptr, key);

    else return NULL;
}

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
        symTableInsert(&(*tree)->lptr, key, data);
        return;
    }
    else if (strCmpString(&key, &(*tree)->key) > 0){
        symTableInsert(&(*tree)->rptr, key, data);
        return;
    } 
}

/**
 *  @brief Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu
 **/
void replaceByRightmost(symTableNodePtr replacedPtr, symTableNodePtr *tree){
    if(!(*tree)) return;

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
    if(!(*tree)) return;

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
    if(!(*tree)) return;

    symTableDispose(&(*tree)->lptr);
    symTableDispose(&(*tree)->rptr);
    free(*tree);
    *tree = NULL;
}

void funcTableInit(funcTableNodePtr *funcTree){
    *funcTree = NULL;
}

funcTableDataPtr funcTableSearch(funcTableNodePtr *funcTree, string key){
    if(!(*funcTree)) return NULL;

    else if (strCmpString(&key, &(*funcTree)->key) < 0)
        return funcTableSearch(&(*funcTree)->lptr, key);
    
    else if (strCmpString(&key, &(*funcTree)->key) > 0)
        return funcTableSearch(&(*funcTree)->rptr, key);

    else return NULL;
}

void funcTableInsert(funcTableNodePtr *funcTree, string key, funcTableDataPtr data){
    if(!(*funcTree)){
        funcTableNodePtr temp = malloc(sizeof(struct funcTableNode));
        if(temp == NULL) return;

        temp->data = data;
        temp->key = key;
        temp->lptr = NULL;
        temp->rptr = NULL;

        return;
    }

    if (strCmpString(&key, &(*funcTree)->key) < 0){
        funcTableInsert(&(*funcTree)->lptr, key, data);
        return;
    }
    else if (strCmpString(&key, &(*funcTree)->key) > 0){
        funcTableInsert(&(*funcTree)->rptr, key, data);
        return;
    } 
}

void replaceByRightmostFunc(funcTableNodePtr replacedPtr, funcTableNodePtr *funcTree){
    if(!(*funcTree)) return;

    if((*funcTree)->rptr != NULL){
        replaceByRightmostFunc(replacedPtr, &(*funcTree)->rptr);
    } else{
        replacedPtr->data = (*funcTree)->data;
        replacedPtr->key = (*funcTree)->key;
        funcTableNodePtr tmp = (*funcTree);
        (*funcTree) = (*funcTree)->lptr;
        free(tmp);
    }
}

void funcTableDelete(funcTableNodePtr *funcTree, string key){
    if(!(*funcTree)) return;

    if (strCmpString(&key, &(*funcTree)->key) < 0){
        funcTableDelete(&(*funcTree)->lptr, key);
    }
    else if (strCmpString(&key, &(*funcTree)->key) > 0){
        funcTableDelete(&(*funcTree)->rptr, key);
    }
    else{
        replaceByRightmostFunc((*funcTree), (&(*funcTree)->lptr));
        if((*funcTree)->rptr == NULL)
            (*funcTree) = (*funcTree)->lptr;
        else
            (*funcTree) = (*funcTree)->rptr;
    }
}

void funcTableDispose(funcTableNodePtr *funcTree){
    if(!(*funcTree)) return;

    funcTableDispose(&(*funcTree)->lptr);
    funcTableDispose(&(*funcTree)->rptr);
    free(*funcTree);
    *funcTree = NULL;
}

/**
 * @brief Funkce kontrolujici typ ID  
 * -1 ... VAR
 * 0  ... NONDEC
 * 1  ... FUNC
 */
int getType(symTableNodePtr *tree, funcTableNodePtr *funcTree, string key){
    if(symTableSearch(tree, key)){
        return -1;
    } else if (funcTableSearch(funcTree, key)){
        return 1;
    } else return 0;
}