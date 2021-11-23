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
    symTableInit(&funcTable);
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    syntax_program();

    strFree(&attribute);
    symTableDispose(&funcTable);
    return SYNTAX_OK;  
}

//// každá syntax funkce je jeden neterminál v gramatice ////

// <program> -> <prolog> <fun_dec_def_call> EOF
void syntax_program(){
    printf("program\n");
    if (token != EOFILE){
        syntax_prolog();
        syntax_fun_dec_def_call();
    }
    //TODO if token != EOFILE => SYNTAX / SEMANTICS ERROR
}

// <prolog> -> KW_REQUIRE "ifj21"
void syntax_prolog(){
    printf("prolog\n");
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != STRING || strCmpConstStr(&attribute, "\"ifj21\"")) errorMessage(ERR_SYNTAX, "Klíčové slovo require musí následovat řetězec \"ifj21\"");
}

// <fun_dec_def_call> -> KW_GLOBAL  ID DOUBLEDOT    KW_FUNC         LBR <param_type>    RBR     <type_rtrn>
// <fun_dec_def_call> -> KW_FUNC    ID LBR          <fun_params>    RBR <type_rtrn>     <stmts> KW_END
// <fun_dec_def_call> -> <fun_call>
// <fun_dec_def_call> -> epsilon
void syntax_fun_dec_def_call(){
    printf("fun_dec_def_call\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token == KW_GLOBAL){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX /*MOZNA ERR_SYNTAX TODO nejsem si jisty jaky tady bude error -xhlins01*/, "Pokus o definici non-ID");
        //je atribut jiz declared?
        if(symTableSearch(&funcTable, attribute)){
            errorMessage(ERR_NONDEF, "Pokus o redeklaraci funkce");
        } else{
            symTableInsert(&funcTable, attribute, NULL);
        }

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak : v deklaraci funkce");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_FUNC) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo function v deklaraci funkce");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak ( v deklaraci funkce");
        syntax_param_type();

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ) v deklaraci funkce");
        syntax_type_rtrn();
    }
    else if (token == KW_FUNC){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if (token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");

        // TODO sémantika
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak (");

        syntax_fun_params();
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak )");

        syntax_type_rtrn();
        syntax_stmts();

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo end na konci funkce");
    }
    else if(token == ID){
        syntax_fun_call();
    }
    //else{
    //    if(token != EOFILE) errorMessage(ERR_SYNTAX, "Program neobsahuje EOF");
    //}
}

// <fun_call> -> ID LBR <fun_call_params> RBR
void syntax_fun_call(){
    printf("fun_call\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token == LBR){
        syntax_fun_call_params();
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se se znak ')'");
    }
    else{
        errorMessage(ERR_SYNTAX, "Očekával se znak '('");
    }
}

// <param_type> -> <type> <param_type2>
// <param_type> -> epsilon
void syntax_param_type(){
    printf("param_type\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(syntax_type()){
        syntax_param_type2();
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(!syntax_type()) errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <param_type2> -> COMMA <type> <param_type2>
// <param_type2> -> epsilon
void syntax_param_type2(){
    printf("param_type2\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_param_type2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak  ','");
}

// <type_rtrn> -> DOUBLEDOT <type> <type_rtrn2>
// <type_rtrn> -> epsilon
void syntax_type_rtrn(){
    printf("type_rtrn\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == DOUBLEDOT){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
}

// <type_rtrn2> -> COMMA <type> <type_rtrn2>
// <type_rtrn2> -> epsilon
void syntax_type_rtrn2(){
    printf("type_rtrn2\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak ','");
}

// <fun_params> -> ID DOUBLEDOT <type> <fun_params2>
// <fun_params> -> epsilon
void syntax_fun_params(){
    printf("fun_params\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT){
            token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(syntax_type()) syntax_fun_params2();
            else errorMessage(ERR_SYNTAX, "Očekával se typ funkce");
        }
        else errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
}

// <fun_params2> -> COMMA ID DOUBLEDOT <type> <fun_params2>
// <fun_params2> -> epsilon
void syntax_fun_params2(){
    printf("fun_params2\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == ID){
            token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(token == DOUBLEDOT){
                token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(syntax_type()) syntax_fun_params2();
                else errorMessage(ERR_SYNTAX, "Očekával se typ funkce");
            }
            else errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
        }
        else errorMessage(ERR_SYNTAX, "Očekávalo se ID");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak ','");
}

// <fun_call_params> -> ID <fun_call_params2>
// <fun_call_params> -> epsilon
void syntax_fun_call_params(){
    printf("fun_call_params\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID) {syntax_fun_call_params2(); token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));}
    //else if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
}

// <fun_call_params2> -> COMMA ID <fun_call_params2>
// <fun_call_params2> -> epsilon
void syntax_fun_call_params2(){
    printf("fun_call_params2\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == ID) syntax_fun_call_params2();
        else errorMessage(ERR_SYNTAX, "Očekávalo se ID");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //else if(token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak ','");
}

// <stmts> -> <stmt> <stmts>
// <stmts> -> epsilon
void syntax_stmts(){
    printf("stmts\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute)); // Absolutně si nejsem jistý tady - Karlos    Jde to videt kappa -xhlins01 🟩☕
    if(token == KW_LOCAL || token == ID || token == KW_IF || token == KW_WHILE || token == KW_RETURN) {syntax_stmt(); token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));}
    //else if(token != KW_LOCAL || token != ID || token != KW_IF || token != KW_WHILE || token != KW_RETURN ) errorMessage(ERR_SYNTAX, "Očekával se příkaz");
}

// <stmt> -> KW_LOCAL   ID          DOUBLEDOT   <type>  <var_init>
// <stmt> -> ID         <ID_next>   ASSIGN      <expr>  <expr2>
// <stmt> -> KW_IF      <cond>      KW_THEN     <stmts> KW_ELSE <stmts> KW_END
// <stmt> -> KW_WHILE   <cond>      KW_DO       <stmts> KW_END
// <stmt> -> ID         <ID_next>   ASSIGN      <fun_call>
// <stmt> -> <fun_call>
// <stmt> -> KW_RETURN  <expr>      <expr2>
void syntax_stmt(){
    printf("stmt\n");
    //TODO
}

// <var_init> -> ASSIGN <expr>
// <var_init> -> epsilon
void syntax_var_init(){
    printf("var_init\n");
    //TODO
}

// <expr> -> <fun_call>
void syntax_expr(){
    printf("expr\n");
    //TODO
}

// <expr2> -> COMMA <expr> <expr2>
// <expr2> -> epsilon
void syntax_expr2(){
    printf("expr2\n");
    //TODO
}

// <type> -> KW_STR
// <type> -> KW_INT
// <type> -> KW_NUM
bool syntax_type(){
    printf("type\n");
    if(token == KW_STR || token == KW_INT || token == KW_NUM) return true;
    else return false;
}

// <ID_next> -> COMMA ID <ID_next>
// <ID_next> -> epsilon
void syntax_ID_next(){
    printf("ID_next\n");
    //TODO
}