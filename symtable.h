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
    struct symTableDataPtr *data;
    struct symTableNode *lptr;
    struct symTableNode *rptr; 
} *symTableNodePtr;

/**
 *  @brief Inicializace BVS 
 **/
void symTableInit(symTableNodePtr *tree);

/**
 *  @brief Hledání uzlu ve stromě podle identifikátoru
 **/
symTableDataPtr symTableSearch(symTableNodePtr *tree, string key);

/**
 *  @brief Vložení nového uzlu do stromu
 **/
void symTableInsert(symTableNodePtr *tree, string key, symTableDataPtr data);

/**
 *  @brief Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu
 **/
void replaceByRightmost(symTableNodePtr replacedPtr, symTableNodePtr *tree);

/**
 *  @brief Vymazání uzlu ze stromu
 **/
void symTableDelete(symTableNodePtr *tree, string key);

/**
 *  @brief Zrušení celého stromu
 **/
void symTableDispose(symTableNodePtr *tree);