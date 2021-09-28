/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 * 
 * Soubor:
 *      @file error.c
 * 
 * Popis:
 *      @brief Výpis chybových hlášek na standartní chybový výstup.
 */

#include "error.h"

void errorMessage(int eNumber, char* eMessage){
    fprintf(stderr, "Nastala chyba [%d]:\n", eNumber);
    switch(eNumber){
        case ERR_LEXICAL:
            fprintf(stderr, "Chyba v programu v rámci lexikální analýzy.\n");
            break;
        case ERR_SYNTAX:
            fprintf(stderr, "Chyba v programu v rámci syntaktické analýzy.\n");
            break;
        case ERR_NONDEF:
            fprintf(stderr, "Sémantická chyba v programu.\n");
            break;
        case ERR_ASSIGN:
            fprintf(stderr, "Sémantická chyba v příkazu přiřazení.\n");
            break;
        case ERR_RETURN:
            fprintf(stderr, "Sémantická chyba v programu.\n");
            break;
        case ERR_TYPE_CMP:
            fprintf(stderr, "Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.\n");
            break;
        case ERR_SEM_OTHER:
            fprintf(stderr, "Ostatní sémantické chyby.\n");
            break;
        case ERR_RUNTIME_NIL:
            fprintf(stderr, "Běhová chyba při práci s neočekávanou hodnotou nil.\n");
            break;
        case ERR_RUNTIME_DIV:
            fprintf(stderr, "Běhová chyba celočíselného dělení nulovou konstantou.\n");
            break;
        case ERR_INTERNAL:
            fprintf(stderr, "Interní chyba překladače.\n");
            break;
    }
    fprintf(stderr, "Další informace o chybě:\n%s\n", eMessage);
    exit(eNumber);
}