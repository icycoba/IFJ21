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


//string token;
//int state;
int token = 0;
string attribute;
symTableNodePtr funcTable;


int parser(){
    int internalErr = 0;
    printf("%d\n", internalErr);

    symTableInit(&funcTable);    

    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");

    token = getToken(&attribute);
    syntax_program();

    return 0;  
}


//každá syntax funkce je jeden neterminál v gramatice
void syntax_program(){
    if (token != EOFILE){
        syntax_prolog();
        syntax_fun_dec_def_call();
    }
    //TODO if token != EOFILE => SYNTAX / SEMANTICS ERROR
}

void syntax_prolog(){
    //TODO komentáře, blokové komentáře
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute);
    if (token != STREND || strCmpConstStr(&attribute, "\"ifj21\"")) errorMessage(ERR_SYNTAX, "Klíčové slovo require musí následovat řetězec \"ifj21\"");


}

void syntax_fun_dec_def_call(){
    token = getToken(&attribute);
    if (token == KW_GLOBAL){
        /* code */
    }
    else if (token == KW_FUNC){
        /* code */
    }
    else if(token == ID){
        syntax_fun_call();
    }
    else{
        if(token != EOFILE) errorMessage(ERR_SYNTAX, "Program neobsahuje EOF");
    }
}

void syntax_fun_call(){
    //TODO
}

void syntax_param_type(){
    //TODO
}
void syntax_param_type2(){
    //TODO
}
void syntax_type_rtrn(){
    //TODO
}
void syntax_type_rtrn2(){
    //TODO
}
void syntax_fun_params(){
    //TODO
}
void syntax_fun_params2(){
    //TODO
}
void syntax_stmts(){
    //TODO
}
void syntax_stmt(){
    //TODO
}
void syntax_var_init(){
    //TODO
}
void syntax_expr(){
    //TODO
}
void syntax_expr2(){
    //TODO
}
void syntax_type(){
    //TODO
}
void syntax_ID_next(){
    //TODO
}