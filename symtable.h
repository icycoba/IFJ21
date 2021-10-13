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
    T_FLOAT,
    T_STRING,
    T_FUNC    
} sType;

/**
 *  @brief Data uzlu 
 **/
typedef struct symTableData {
    sType type;
    char* attribute;
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

void symTableInit(symTableNodePtr *tree);
symTableDataPtr symTableSearch(symTableNodePtr *tree, string key);
void symTableInsert(symTableNodePtr *tree, string key, symTableDataPtr data);
void replaceByRightmost(symTableNodePtr replacedPtr, symTableNodePtr *tree);
void symTableDelete(symTableNodePtr *tree, string key);
void symTableDispose(symTableNodePtr *tree);