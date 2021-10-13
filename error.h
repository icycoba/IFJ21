/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 * 
 * Soubor:
 *      @file error.h
 * 
 * Popis:
 *      @brief Výpis chybových hlášek na standartní chybový výstup.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ERR_LEXICAL     1   // Chyba v programu v rámci lexikální analýzy
#define ERR_SYNTAX      2   // Chyba v programu v rámci syntaktické analýzy
#define ERR_NONDEF      3   // Nedefinovaná funkce/proměnná, pokus o redefinici proměnné, ...
#define ERR_ASSIGN      4   // Chyba v příkazu přiřazení (typová nekompatibilita)
#define ERR_RETURN      5   // Špatný počet/typ parametrů či návratových hodnot u volání či návratu z funkce
#define ERR_TYPE_CMP    6   // Chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
#define ERR_SEM_OTHER   7   // Ostatní sémantické chyby
#define ERR_RUNTIME_NIL 8   // Běhová chyba při práci s neočekávanou hodnotou nil
#define ERR_RUNTIME_DIV 9   // Běhová chyba celočíselného dělení nulovou konstantou
#define ERR_INTERNAL    99  // Interní chyba překladače (např. chyba alokace paměti, ...)

void errorMessage(int eNumber, char* eMessage);