/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
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

typedef enum{
    T_NIL,
    T_INT,
    T_DOUBLE,
    T_STRING
} sType;

/**
 *  @brief Data uzlu 
 **/
typedef struct varTableData {
    sType type;
    string attribute;
    int scope;
} *varTableDataPtr;

/**
 *  @brief Uzel stromu
 **/
typedef struct varTableNode {
    string key;
    struct varTableData *data;
    struct varTableNode *lptr;
    struct varTableNode *rptr; 
} *varTableNodePtr;

typedef struct funcTableData{
    //TODO seznam parametru, jejich typu
    //TODO -II- navratove typy
    bool defined;
    bool declared;
    int paramCount;
    int returnParamCount;
} *funcTableDataPtr;

typedef struct funcTableNode{
    string key;
    struct funcTableData *data;
    struct funcTableNode *lptr;
    struct funcTableNode *rptr;
} *funcTableNodePtr;

void varTableInit(varTableNodePtr *tree);
varTableDataPtr varTableSearch(varTableNodePtr *tree, string key);
void varTableInsert(varTableNodePtr *tree, string key, varTableDataPtr data);
void replaceByRightmost(varTableNodePtr replacedPtr, varTableNodePtr *tree);
void varTableDelete(varTableNodePtr *tree, string key);
void varTableDispose(varTableNodePtr *tree);

void funcTableInit(funcTableNodePtr *funcTree);
funcTableDataPtr funcTableSearch(funcTableNodePtr *funcTree, string key);
void funcTableInsert(funcTableNodePtr *funcTree, string key, funcTableDataPtr data);
void replaceByRightmostFunc(funcTableNodePtr replacedPtr, funcTableNodePtr *funcTree);
void funcTableDelete(funcTableNodePtr *funcTree, string key);
void funcTableDispose(funcTableNodePtr *funcTree);

// Nejsem si jisty jestli tohle bude potreba, ale pro jistotu
void varTypeAdd(varTableNodePtr *tree, string key, sType type);
void varAttributeAdd(varTableNodePtr *tree, string key, string attribute);

// Pridani parametru, navratovych hodnot a jejich typu
void funcParamsAdd();
void funcReturnParamsAdd();
void funcParamsTypesAdd();
void funcReturnParamsTypesAdd();

//bool isDeclared(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key);
//bool isFunc(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key);

int getType(varTableNodePtr *tree, funcTableNodePtr *funcTree, string key);