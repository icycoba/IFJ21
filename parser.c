/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      
 * 
 * Soubor:
 *      @file parser.c
 * 
 * Popis:
 *      @brief Modul pro provedení syntaktické a sémantické analýzy
 */

#include "parser.h"


string token;
int state;


int parser(){
    int internalErr = 0;
    printf("%d\n", internalErr);

    symTableNodePtr funcTable;
    symTableInit(&funcTable);    

    
    state = getToken(&token);
    syntax_program();

    return 0;  
}


//každá syntax funkce je jeden neterminál v gramatice
int syntax_program(){
    syntax_prolog();
    syntax_fun_dec_def_call();

}

int syntax_prolog(){
    
    if (state != KW_REQUIRE) return ERR_SYNTAX;

    if ((state = getToken(&token)) == ERR_LEXICAL) return ERR_LEXICAL;

    if (state != STREND || strCmpStr(token, "\"ijf21\"")) return ERR_SYNTAX;

    return SYNTAX_OK;

}

int syntax_fun_dec_def_call(){

}