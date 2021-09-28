/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      Převzato ze stránek IFJ projektu. (http://www.fit.vutbr.cz/study/courses/IFJ/public/project/)
 * 
 * Soubor:
 *      @file str.c
 * 
 * Popis:
 *      @brief Jednoduchá knihovna pro práci s nekonečně dlouhými řetězci.
 */

#include "str.h"

#define STR_LEN_INC 8
//  Konstanta udává, na kolik bytů provedeme počáteční alokaci paměti, pokud načítáme
//  řetězec znak po znaku, paměť se postupně bude alokovat na násobky
//  tohoto čísla.

#define STR_ERROR   1
#define STR_SUCCESS 0

//  Vytvoří nový řetězec
int strInit(string *s){
    if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL) return STR_ERROR;

    s->str[0] = '\0';
    s->length = 0;
    s->allocSize = STR_LEN_INC;
    return STR_SUCCESS;
}

//  Uvolní řetězec z paměti
void strFree(string *s){
    free(s->str);
}

//  Vymaže obsah řetězce
void strClear(string *s){
    s->str[0] = '\0';
    s->length = 0;
}

//  Přidá na konec řetězce jeden znak
int strAddChar(string *s1, char c){
    if(s1->length + 1 >= s1->allocSize){
        if((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL) return STR_ERROR;
        s1->allocSize = s1->length + STR_LEN_INC;
    }
    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
    return STR_SUCCESS;
}

//  Překopíruje řetězec s2 do s1
int strCopyString(string *s1, string *s2){
    int newLength = s2->length;
    if(newLength >= s1->allocSize){
        if((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL) return STR_ERROR;
        s1->allocSize = newLength + 1;
    }
    strcpy(s1->str, s2->str);
    s1->length = newLength;
    return STR_SUCCESS;
}

//  Porovná oba řetězce a vrátí výsledek
int strCmpString(string *s1, string *s2){
    return strcmp(s1->str, s2->str);
}

//  Porovná náš řetězec s konstantním řetězcem
int strCmpConstStr(string *s1, char* s2){
    return strcmp(s1->str, s2);
}

//  Vrátí textovou část řetězce
char *strGetStr(string *s){
    return s->str;
}

//  Vrátí délku daného řetězce
int strGetLength(string *s){
    return s->length;
}