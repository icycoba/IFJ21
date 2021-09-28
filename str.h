/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      Převzato ze stránek IFJ projektu. (http://www.fit.vutbr.cz/study/courses/IFJ/public/project/)
 * 
 * Soubor:
 *      @file str.h
 * 
 * Popis:
 *      @brief Jednoduchá knihovna pro práci s nekonečně dlouhými řetězci.
 */

#include <string.h>
#include <malloc.h>

typedef struct {
    char* str;      // Místo pro daný řetězec ukončený znakem '\0'
    int length;     // Skutečná délka řetězce
    int allocSize;  // Velikost alokované paměti
} string;

int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);