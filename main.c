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
    /*FILE *f;
    if ((f = fopen("ifj21.tl", "r")) == NULL){
        printf("Soubor se nepodarilo otevrit.\n");
        return 1000000; // TODO remove me
    }
    printf("---\nSoubor otevřen úspěšně:)\n---\n");
    */
    int returnCode = 0;
    string attribute;
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    printf("Hello karel\n\n");

    int returnCodeTest = 0;

    while (returnCodeTest != 999){
        returnCodeTest = getToken(&attribute);
        if (returnCodeTest > S_ZERO && returnCodeTest < 999 && (returnCodeTest != COMM_LINE_END && returnCodeTest != BLOCK_END)){
            printf("%-15s |%s\n", printState(returnCodeTest), strGetStr(&attribute));
        }
        //if (attribute.length != 0){
        //    fprintf(stderr, "%s\n", strGetStr(&attribute));
        //}
        strClear(&attribute);
    }

    //int returnCode = parser();
    return returnCode;
}