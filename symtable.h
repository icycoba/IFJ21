/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 *      @author Jakub Ryšánek   (xrysan05)
 * 
 * Soubor:
 *      @file symtable.h
 * 
 * Popis:
 *      @brief Tabulka symbolů implementovaná jako binární vyhledávací strom.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "str.h"
#include "list.h"
#include "scanner.h"



/**
 *  @brief Uzel stromu
 **/
typedef struct varTableNode {
    string key;
    string type;
    int scope;
    struct varTableNode *lptr;
    struct varTableNode *rptr; 
} *varTableNodePtr;


typedef struct funcTableNode{
    string key;
    bool defined;
    bool declared;
    bool returned;
    DLList param;
    DLList returnParam;
    struct funcTableNode *lptr;
    struct funcTableNode *rptr;
} *funcTableNodePtr;

void varTableInit(varTableNodePtr *tree);
varTableNodePtr varTableSearch(varTableNodePtr *tree, string key);
void varTableInsert(varTableNodePtr *tree, string key);
void varTypeAdd(varTableNodePtr *tree, string key, string type);
void scopeAdd(varTableNodePtr *tree);
void scopeSub(varTableNodePtr *tree);
void replaceByRightmost(varTableNodePtr replacedPtr, varTableNodePtr *tree);
void varTableDelete(varTableNodePtr *tree, string key);
void varTableDispose(varTableNodePtr *tree);

void funcTableInit(funcTableNodePtr *funcTree);
funcTableNodePtr funcTableSearch(funcTableNodePtr *funcTree, string key);
void funcTableInsert(funcTableNodePtr *funcTree, string key);
void funcDefined(funcTableNodePtr *funcTree, string key);
void replaceByRightmostFunc(funcTableNodePtr replacedPtr, funcTableNodePtr *funcTree);
void funcTableDelete(funcTableNodePtr *funcTree, string key);
void funcTableDispose(funcTableNodePtr *funcTree);
void controlDefined(funcTableNodePtr *funcTree);

// Nejsem si jisty jestli tohle bude potreba, ale pro jistotu

// Pridani parametru, navratovych hodnot a jejich typu
void funcParamsAdd(funcTableNodePtr *funcTree, string key);
void funcReturnParamsAdd(funcTableNodePtr *funcTree, string key);
void funcParamsTypesAdd(funcTableNodePtr *funcTree, string key);
void funcReturnParamsTypesAdd(funcTableNodePtr *funcTree, string key);

void addParam(funcTableNodePtr *funcTree, string key, string param);
void addReturnParam(funcTableNodePtr *funcTree, string key, string returnParam);
void funcDefined(funcTableNodePtr *funcTree, string key);
void funcDeclared(funcTableNodePtr *funcTree, string key);

//bool isDeclared(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key);
//bool isFunc(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key);

int getType(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key);
void simple_print(varTableNodePtr *tree);
void simple_print2(funcTableNodePtr *tree);