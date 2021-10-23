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
    int returnCode = 0;
    string attribute;
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    printf("Hello karel\n\n");

    int returnCodeTest = 0;

    while (returnCodeTest != 999){
        returnCodeTest = getToken(&attribute);
        if (returnCodeTest > S_ZERO && returnCodeTest < EOFILE && (returnCodeTest != COMM_LINE_END && returnCodeTest != BLOCK_END)){
            printf("%-15s |%s\n", printState(returnCodeTest), strGetStr(&attribute));
        }
        strClear(&attribute);
    }
    return returnCode;
}