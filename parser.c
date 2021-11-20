/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek   (xrysan05)
 *      @author Karel Galeta    (xgalet05)
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
    if (state != EOFILE){
        syntax_prolog();
        syntax_fun_dec_def_call();
    }
    return SYNTAX_OK;

}

int syntax_prolog(){
    
    if (state != KW_REQUIRE) return ERR_SYNTAX;

    if ((state = getToken(&token)) == ERR_LEXICAL) return ERR_LEXICAL;

    if (state != STREND || strCmpStr(token, "\"ijf21\"")) return ERR_SYNTAX;

    return SYNTAX_OK;

}

int syntax_fun_dec_def_call(){
    state = getToken(&token);
    if (state == KW_GLOBAL){
        /* code */
    }
    else if (state == KW_FUNC){
        /* code */
    }
    else if(state == ID){
        syntax_fun_call();
    }
    else{
        if(state != EOFILE) return ERR_SYNTAX;
        return SYNTAX_OK;
    }
}

syntax_fun_call(){

}