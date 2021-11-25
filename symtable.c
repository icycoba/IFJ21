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
void varTableInit(varTableNodePtr *tree){
    *tree = NULL;
}

/**
 *  @brief Hledání uzlu ve stromě podle identifikátoru
 **/
varTableDataPtr varTableSearch(varTableNodePtr *tree, string key){
    if(!(*tree)) return NULL;

    else if (strCmpString(&key, &(*tree)->key) < 0)
        return varTableSearch(&(*tree)->lptr, key);
    
    else if (strCmpString(&key, &(*tree)->key) > 0)
        return varTableSearch(&(*tree)->rptr, key);

    else return NULL;
}

/**
 *  @brief Vložení nového uzlu do stromu
 **/
void varTableInsert(varTableNodePtr *tree, string key, varTableDataPtr data){
    if(!(*tree)){
        varTableNodePtr temp = malloc(sizeof(struct varTableNode));
        if(temp == NULL) return;

        temp->data = data;
        temp->key = key;
        temp->lptr = NULL;
        temp->rptr = NULL;

        return;
    }

    if (strCmpString(&key, &(*tree)->key) < 0){
        varTableInsert(&(*tree)->lptr, key, data);
        return;
    }
    else if (strCmpString(&key, &(*tree)->key) > 0){
        varTableInsert(&(*tree)->rptr, key, data);
        return;
    } 
}

/**
 *  @brief Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu
 **/
void replaceByRightmost(varTableNodePtr replacedPtr, varTableNodePtr *tree){
    if(!(*tree)) return;

    if((*tree)->rptr != NULL){
        replaceByRightmost(replacedPtr, &(*tree)->rptr);
    } else{
        replacedPtr->data = (*tree)->data;
        replacedPtr->key = (*tree)->key;
        varTableNodePtr tmp = (*tree);
        (*tree) = (*tree)->lptr;
        free(tmp);
    }
}

/**
 *  @brief Vymazání uzlu ze stromu
 **/
void varTableDelete(varTableNodePtr *tree, string key){
    if(!(*tree)) return;

    if (strCmpString(&key, &(*tree)->key) < 0){
        varTableDelete(&(*tree)->lptr, key);
    }
    else if (strCmpString(&key, &(*tree)->key) > 0){
        varTableDelete(&(*tree)->rptr, key);
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
void varTableDispose(varTableNodePtr *tree){
    if(!(*tree)) return;

    varTableDispose(&(*tree)->lptr);
    varTableDispose(&(*tree)->rptr);
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
int getType(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key){
    if(varTableSearch(&(*tree), key)){
        return -1;
    } else if (funcTableSearch(&(*funcTree), key)){
        return 1;
    } else return 0;
}

void varTypeAdd(varTableNodePtr *tree, string key, sType type){
    (*tree) = varTableSearch(&(*tree), key);
    (*tree)->data->type = type;
}

void varAttributeAdd(varTableNodePtr *tree, string key, string attribute){
    (*tree) = varTableSearch(&(*tree), key);
    (*tree)->data->attribute = attribute;
}