/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek   (xrysan05)
 *      @author Karel Galeta    (xgalet05)
 *      @author Martin Hlinský  (xhlins01)
 * 
 * Soubor:
 *      @file parser.c
 * 
 * Popis:
 *      @brief Modul pro provedení syntaktické a sémantické analýzy
 */

#include "parser.h"

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
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute);
    if (token != STRING || strCmpConstStr(&attribute, "\"ifj21\"")) errorMessage(ERR_SYNTAX, "Klíčové slovo require musí následovat řetězec \"ifj21\"");
}

// <fun_dec_def_call> -> KW_GLOBAL  ID DOUBLEDOT    KW_FUNC         LBR <param_type>    RBR     <type_rtrn>
// <fun_dec_def_call> -> KW_FUNC    ID LBR          <fun_params>    RBR <type_rtrn>     <stmts> KW_END
// <fun_dec_def_call> -> <fun_call>
// <fun_dec_def_call> -> epsilon
void syntax_fun_dec_def_call(){
    token = getToken(&attribute);
    if (token == KW_GLOBAL){
        token = getToken(&attribute);
        if(token != ID) errorMessage(ERR_TYPE_CMP /*MOZNA ERR_SYNTAX TODO nejsem si jisty jaky tady bude error -xhlins01*/, "Pokus o definici non-ID");
        //je atribut jiz declared?
        if(symTableSearch(&funcTable, attribute)){
            errorMessage(ERR_NONDEF, "Pokus o redeklaraci funkce");
        } else{
            symTableInsert(&funcTable, attribute, NULL);
        }

        token = getToken(&attribute);
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak : v deklaraci funkce");

        token = getToken(&attribute);
        if(token != KW_FUNC) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo function v deklaraci funkce");

        token = getToken(&attribute);
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak ( v deklaraci funkce");
        syntax_param_type();

        token = getToken(&attribute);
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ) v deklaraci funkce");
        syntax_type_rtrn();
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
    token = getToken(&attribute);
    if (token == LBR){
        syntax_fun_call_params();
        token = getToken(&attribute);
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se se znak ')'");
    }
    else{
        errorMessage(ERR_SYNTAX, "Očekával se znak '('");
    }
}

// <param_type> -> <type> <param_type2>
// <param_type> -> epsilon
void syntax_param_type(){
    token = getToken(&attribute);
    if(syntax_type()){
        syntax_param_type2();
    }
    else if(!syntax_type()) errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <param_type2> -> COMMA <type> <param_type2>
// <param_type2> -> epsilon
void syntax_param_type2(){
    token = getToken(&attribute);
    if(token == COMMA){
        token = getToken(&attribute);
        if(syntax_type()) syntax_param_type2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
    else if(token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak  ','");
}

// <type_rtrn> -> DOUBLEDOT <type> <type_rtrn2>
// <type_rtrn> -> epsilon
void syntax_type_rtrn(){
    token = getToken(&attribute);
    if(token == DOUBLEDOT){
        token = getToken(&attribute);
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
    else if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
}

// <type_rtrn2> -> COMMA <type> <type_rtrn2>
// <type_rtrn2> -> epsilon
void syntax_type_rtrn2(){
    token = getToken(&attribute);
    if(token == COMMA){
        token = getToken(&attribute);
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
    else if(token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak ','");
}

// <fun_params> -> ID DOUBLEDOT <type> <fun_params2>
// <fun_params> -> epsilon
void syntax_fun_params(){
    token = getToken(&attribute);
    if(token == ID){
        token = getToken(&attribute);
        if(token == DOUBLEDOT){
            token = getToken(&attribute);
            if(syntax_type()) syntax_fun_call_params2();
            else errorMessage(ERR_SYNTAX, "Očekával se typ funkce");
        }
        else errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
    }
    else if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
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
bool syntax_type(){
    if(token == KW_STR || token == KW_INT || token == KW_NUM) return true;
    else return false;
}

// <ID_next> -> COMMA ID <ID_next>
// <ID_next> -> epsilon
void syntax_ID_next(){
    //TODO
}