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


int parser(){
    int internalErr = 0;
    printf("%d\n", internalErr);

    symTableInit(&funcTable);    

    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");

    token = getToken(&attribute);
    syntax_program();

    return 0;  
}

//// každá syntax funkce je jeden neterminál v gramatice ////

// <program> -> <prolog> <fun_dec_def_call> EOF
void syntax_program(){
    if (token != EOFILE){
        syntax_prolog();
        syntax_fun_dec_def_call();
    }
    //TODO if token != EOFILE => SYNTAX / SEMANTICS ERROR
}

// <prolog> -> KW_REQUIRE "ifj21"
void syntax_prolog(){
    //TODO START komentáře, blokové komentáře

    // while (token == COMM_LINE_END || token == BLOCK_END){}
    // NEBO
    // if (token == COMM_LINE_END || token == BLOCK_END){
    //  syntax_prolog();
    //  return 0;
    // }

    // TODO END
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute);
    if (token != STREND || strCmpConstStr(&attribute, "\"ifj21\"")) errorMessage(ERR_SYNTAX, "Klíčové slovo require musí následovat řetězec \"ifj21\"");


}

// <fun_dec_def_call> -> KW_GLOBAL  ID DOUBLEDOT    KW_FUNC         LBR <param_type>    RBR     <type_rtrn>
// <fun_dec_def_call> -> KW_FUNC    ID LBR          <fun_params>    RBR <type_rtrn>     <stmts> KW_END
// <fun_dec_def_call> -> <fun_call>
// <fun_dec_def_call> -> epsilon
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

// <fun_call> -> ID LBR <fun_call_params> RBR
void syntax_fun_call(){
    //TODO
}

// <param_type> -> <type> <param_type2>
// <param_type> -> epsilon
void syntax_param_type(){
    //TODO
}

// <param_type2> -> COMMA <type> <param_type2>
// <param_type2> -> epsilon
void syntax_param_type2(){
    //TODO
}

// <type_rtrn> -> DOUBLEDOT <type> <type_rtrn2>
// <type_rtrn> -> epsilon
void syntax_type_rtrn(){
    //TODO
}

// <type_rtrn2> -> COMMA <type> <type_rtrn2>
// <type_rtrn2> -> epsilon
void syntax_type_rtrn2(){
    //TODO
}

// <fun_params> -> ID DOUBLEDOT <type> <fun_params2>
// <fun_params> -> epsilon
void syntax_fun_params(){
    //TODO
}

// <fun_params2> -> COMMA ID DOUBLEDOT <type> <fun_params2>
// <fun_params2> -> epsilon
void syntax_fun_params2(){
    //TODO
}

// <fun_call_params> -> ID <fun_call_params2>
// <fun_call_params> -> epsilon
void syntax_fun_call_params(){
    //TODO
}

// <fun_call_params2> -> COMMA ID <fun_call_params2>
// <fun_call_params2> -> epsilon
void syntax_fun_call_params2(){
    //TODO
}

// <stmts> -> <stmt> <stmts>
// <stmts> -> epsilon
void syntax_stmts(){
    //TODO
}

// <stmt> -> KW_LOCAL   ID          DOUBLEDOT   <type>  <var_init>
// <stmt> -> ID         <ID_next>   ASSIGN      <expr>  <expr2>
// <stmt> -> KW_IF      <cond>      KW_THEN     <stmts> KW_ELSE <stmts> KW_END
// <stmt> -> KW_WHILE   <cond>      KW_DO       <stmts> KW_END
// <stmt> -> ID         <ID_next>   ASSIGN      <fun_call>
// <stmt> -> <fun_call>
// <stmt> -> KW_RETURN  <expr>      <expr2>
void syntax_stmt(){
    //TODO
}

// <var_init> -> ASSIGN <expr>
// <var_init> -> epsilon
void syntax_var_init(){
    //TODO
}

// <expr> -> <fun_call>
void syntax_expr(){
    //TODO
}

// <expr2> -> COMMA <expr> <expr2>
// <expr2> -> epsilon
void syntax_expr2(){
    //TODO
}

// <type> -> KW_STR
// <type> -> KW_INT
// <type> -> KW_NUM
void syntax_type(){
    //TODO
}

// <ID_next> -> COMMA ID <ID_next>
// <ID_next> -> epsilon
void syntax_ID_next(){
    //TODO
}