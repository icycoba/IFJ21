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



//--------------------------//
////        proměné       ////
//--------------------------//


/**
 *  @brief Inicializace BVS 
 **/
void varTableInit(varTableNodePtr *tree){
    *tree = NULL;
}

/**
 *  @brief Hledání uzlu ve stromě podle identifikátoru
 **/
varTableNodePtr varTableSearch(varTableNodePtr *tree, string key){
    if(!(*tree)) return NULL;

    else if (strCmpString(&key, &(*tree)->key) < 0)
        return varTableSearch(&(*tree)->lptr, key);
    
    else if (strCmpString(&key, &(*tree)->key) > 0)
        return varTableSearch(&(*tree)->rptr, key);

    else return *tree;
}

/**
 *  @brief Vložení nového uzlu do stromu
 **/
void varTableInsert(varTableNodePtr *tree, string key){
    if((*tree) == NULL){
        varTableNodePtr temp = malloc(sizeof(struct varTableNode));
        if(temp == NULL) return;

        
        string new;
        if(strInit(&new)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
        strCopyString(&new, &key);
        temp->key = new;

        string new2;
        if(strInit(&new2)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
        strAddString(&new2, "nil");

        temp->type = new2;
        temp->scope = 0;
        temp->lptr = NULL;
        temp->rptr = NULL;
        *tree = temp;
        
        
        return;
    }

    else if (strCmpString(&key, &(*tree)->key) < 0){
        varTableInsert(&(*tree)->lptr, key);
        return;
    }
    else if (strCmpString(&key, &(*tree)->key) > 0){
        varTableInsert(&(*tree)->rptr, key);
        return;
    } 
}

void varTypeAdd(varTableNodePtr *tree, string key, string type){
    varTableNodePtr new;
    new = varTableSearch(&(*tree), key);
    if(new != NULL)   new->type = type;
}

void scopeAdd(varTableNodePtr *tree){
    if (*tree != NULL){
        scopeAdd(&(*tree)->lptr);
        (*tree)->scope++;
        scopeAdd(&(*tree)->rptr);
    }

}

void scopeSub(varTableNodePtr *tree){
    if (*tree != NULL){
        scopeSub(&(*tree)->lptr);
        (*tree)->scope--;        
        scopeSub(&(*tree)->rptr);        
        if((*tree)->scope < 0)  varTableDelete(&(*tree), (*tree)->key);
        
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
        //string new;
        //if(strInit(&new)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
        //strCopyString(&new, &(*tree)->key);
        //replacedPtr->key = new;
        replacedPtr->key = (*tree)->key;
        varTableNodePtr tmp = (*tree);
        (*tree) = (*tree)->lptr;
        strFree(&tmp->key);
        strFree(&tmp->type);
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
    strFree(&(*tree)->key);
    strFree(&(*tree)->type);
    free(*tree);
    *tree = NULL;
}


//--------------------------//
////        funkce        ////
//--------------------------//



void funcTableInit(funcTableNodePtr *funcTree){
    *funcTree = NULL;
}

funcTableNodePtr funcTableSearch(funcTableNodePtr *funcTree, string key){
    if(!(*funcTree)) return NULL;

    else if (strCmpString(&key, &(*funcTree)->key) < 0)
        return funcTableSearch(&(*funcTree)->lptr, key);
    
    else if (strCmpString(&key, &(*funcTree)->key) > 0)
        return funcTableSearch(&(*funcTree)->rptr, key);

    else return *funcTree;
}

void funcTableInsert(funcTableNodePtr *funcTree, string key){
    if(!(*funcTree)){
        funcTableNodePtr temp = malloc(sizeof(struct funcTableNode));
        if(temp == NULL) return;

        string new;
        if(strInit(&new)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
        strCopyString(&new, &key);

        temp->key = new;

        DLList param;
        DLL_Init(&param);
        temp->param = param;
        
        DLList returnParam;
        DLL_Init(&returnParam);
        temp->returnParam = returnParam;

        temp->defined = false;
        temp->declared = false;
        temp->lptr = NULL;
        temp->rptr = NULL;
        *funcTree = temp;

        return;
    }

    if (strCmpString(&key, &(*funcTree)->key) < 0){
        funcTableInsert(&(*funcTree)->lptr, key);
        return;
    }
    else if (strCmpString(&key, &(*funcTree)->key) > 0){
        funcTableInsert(&(*funcTree)->rptr, key);
        return;
    } 
}


void addParam(funcTableNodePtr *funcTree, string key, string param){
    funcTableNodePtr new;
    new = funcTableSearch(&(*funcTree), key);
    if(new != NULL) DLL_InsertLast(&new->param, param);
}


void addReturnParam(funcTableNodePtr *funcTree, string key, string returnParam){
    funcTableNodePtr new;
    new = funcTableSearch(&(*funcTree), key);
    if(new != NULL) DLL_InsertLast(&new->returnParam, returnParam);
}



void funcDefined(funcTableNodePtr *funcTree, string key){
    funcTableNodePtr new;
    new = funcTableSearch(&(*funcTree), key);
    if(new != NULL) new->defined = true;

}

void funcDeclared(funcTableNodePtr *funcTree, string key){
    funcTableNodePtr new;
    new = funcTableSearch(&(*funcTree), key);
    if(new != NULL) new->declared= true;

}

void replaceByRightmostFunc(funcTableNodePtr replacedPtr, funcTableNodePtr *funcTree){
    if(!(*funcTree)) return;

    if((*funcTree)->rptr != NULL){
        replaceByRightmostFunc(replacedPtr, &(*funcTree)->rptr);
    } else{
        replacedPtr->defined = (*funcTree)->defined;
        replacedPtr->param = (*funcTree)->param;
        replacedPtr->returnParam = (*funcTree)->returnParam;
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
    DLL_Dispose(&(*funcTree)->param);
    DLL_Dispose(&(*funcTree)->returnParam);
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

sType typeConvertor(int state){
    if (state == KW_INT) return T_INT;
    else if (state == KW_NIL) return T_NIL;
    else if (state == KW_STR) return T_STRING;
    else if (state == KW_NUM) return T_DOUBLE;
    else errorMessage(ERR_SYNTAX, "Chyba ve volani funkce typeConvertor");

}

void simple_print(varTableNodePtr *tree){
    if (*tree != NULL){
        simple_print(&(*tree)->lptr);
        printf("[%s, %d]  ",strGetStr(&(*tree)->key), (*tree)->scope);
        simple_print(&(*tree)->rptr);
    }
}

void simple_print2(funcTableNodePtr *tree){
    if (*tree != NULL){
        simple_print2(&(*tree)->lptr);
        printf("[%s]  ",strGetStr(&(*tree)->key));
        simple_print2(&(*tree)->rptr);
    }
}


