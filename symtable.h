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
typedef struct symTableData {
    sType type;
    string attribute;
    int scope;
} *symTableDataPtr;

/**
 *  @brief Uzel stromu
 **/
typedef struct symTableNode {
    string key;
    struct symTableData *data;
    struct symTableNode *lptr;
    struct symTableNode *rptr; 
} *symTableNodePtr;


typedef struct funcTableData{
    //TODO seznam parametru, jejich typu
    //TODO -II- navratove typy
    bool declared;
} *funcTableDataPtr;

typedef struct funcTableNode{
    string key;
    struct funcTableData *data;
    struct funcTableNode *lptr;
    struct funcTableNode *rptr;
} *funcTableNodePtr;

void symTableInit(symTableNodePtr *tree);
symTableDataPtr symTableSearch(symTableNodePtr *tree, string key);
void symTableInsert(symTableNodePtr *tree, string key, symTableDataPtr data);
void replaceByRightmost(symTableNodePtr replacedPtr, symTableNodePtr *tree);
void symTableDelete(symTableNodePtr *tree, string key);
void symTableDispose(symTableNodePtr *tree);

void funcTableInit(funcTableNodePtr *funcTree);
funcTableDataPtr funcTableSearch(funcTableNodePtr *funcTree, string key);
void funcTableInsert(funcTableNodePtr *funcTree, string key, funcTableDataPtr data);
void replaceByRightmostFunc(funcTableNodePtr replacedPtr, funcTableNodePtr *funcTree);
void funcTableDelete(funcTableNodePtr *funcTree, string key);
void funcTableDispose(funcTableNodePtr *funcTree);

void funcParamsAppend();

//bool isDeclared(symTableNodePtr *tree, funcTableNodePtr *funcTree, string key);
//bool isFunc(symTableNodePtr *tree, funcTableNodePtr *funcTree, string key);

int getType(symTableNodePtr *tree, funcTableNodePtr *funcTree, string key);