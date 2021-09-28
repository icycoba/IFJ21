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

#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "str.h"

typedef enum{
    T_INT,
    T_FLOAT,
    T_STRING,
    T_NIL
} sType;

typedef struct symTableData {
    sType type;
    char* attribute;
} *symTableDataPtr;

typedef struct symTableNode {
    string key;
    struct symTableDataPtr *data;
    struct symTableNode *lptr;
    struct symTableNode *rptr; 
} *symTableNodePtr;