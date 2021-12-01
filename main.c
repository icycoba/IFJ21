/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 * 
 * Soubor:
 *      @file main.c
 * 
 * Popis:
 *      @brief Hlavní program.
 */
#include "main.h"

int main(){
    /*
    int returnCode = 0;
    string attribute;
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");

    int returnCodeTest = 0;

    while (returnCodeTest != EOFILE){
        returnCodeTest = getToken(&attribute);
        if (returnCodeTest > S_ZERO && returnCodeTest < UNKNOWN){
            printf("%-15s |%s\n", printState(returnCodeTest), strGetStr(&attribute));
        }
        
        strClear(&attribute);
        //if (returnCodeTest == EOFILE){
        //    return 999;
        //}
    }
    printf("Navratovy kod je %d\n", returnCode);
    return returnCode;
    */
    if(!parser()) return 0;
    else return ERR_INTERNAL;

    ////            debug veci              /////

    /*varTableNodePtr varTable;
    varTableInit(&varTable);
    string key;
    if(strInit(&key)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    while(getToken(&key) != EOFILE){

     
        varTableInsert(&varTable, key);

        strClear(&key);
    
    
    }
    simple_print(&varTable);
    strAddString(&key, "end");
    printf("\n");
    //if(varTableSearch(&varTable, key) != NULL)printf("chyba");
    varTableDelete(&varTable, key);
    simple_print(&varTable);
    printf("\n");

    scopeAdd(&varTable);

    varTableInsert(&varTable, key);
    simple_print(&varTable);
    printf("\n");
    
    scopeSub(&varTable);
    simple_print(&varTable);
    printf("\n");
    
    strFree(&key);
    varTableDispose(&varTable);*/
}