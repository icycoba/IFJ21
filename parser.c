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

/**
 * @brief  Funkce, která obstarává chod syntaktické a sémantiské analýzy
*/
int parser(){
    symTableInit(&funcTable);
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    syntax_program();

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != EOFILE) errorMessage(ERR_SYNTAX, "Chybí EOF");

    strFree(&attribute);
    symTableDispose(&funcTable);
    return SYNTAX_OK;  
}

void bottom_up(){
    printf("bottom-up\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    while(token != KW_THEN && token != KW_DO && token != EOFILE){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
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

        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
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

        //TODO scuffed cteni tokenu (demonstrovano na maluvce)
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT) syntax_type_rtrn();
        //syntax_type_rtrn();
        syntax_stmts();

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo end na konci funkce");
    }
    else if(token == ID){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_fun_call();
    }
    //else{
    //    if(token != EOFILE) errorMessage(ERR_SYNTAX, "Program neobsahuje EOF");
    //}
}

// <fun_call> -> ID LBR <fun_call_params> RBR
void syntax_fun_call(){
    printf("fun_call\n");    
    if (token == LBR){
        syntax_fun_call_params();
        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se se znak ')'");
    }
    else errorMessage(ERR_SYNTAX, "Očekával se znak '('");
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
    //else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
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
    //else errorMessage(ERR_SYNTAX, "Očekával se znak  ','");
}

// <type_rtrn> -> DOUBLEDOT <type> <type_rtrn2>
// <type_rtrn> -> epsilon
void syntax_type_rtrn(){
    printf("type_rtrn\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(syntax_type()) syntax_type_rtrn2();
    else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
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
    
}

// <fun_call_params> -> ID <fun_call_params2>
// <fun_call_params> -> epsilon
void syntax_fun_call_params(){
    printf("fun_call_params\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID) {
        syntax_fun_call_params2();
        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
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
    
}

// <stmts> -> <stmt> <stmts>
// <stmts> -> epsilon
void syntax_stmts(){
    printf("stmts\n");
    //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == KW_LOCAL || token == ID || token == KW_IF || token == KW_WHILE || token == KW_RETURN || (token < UNKNOWN && token > KW_WHILE)) {
        syntax_stmt(); 
        //token = getToken(&attribute);
        syntax_stmts();
    }
}

// <stmt> -> KW_LOCAL       ID          DOUBLEDOT   <type>      <var_init>
// <stmt> -> KW_IF          term        KW_THEN     <stmts>     KW_ELSE <stmts> KW_END
// <stmt> -> KW_WHILE       term        KW_DO       <stmts>     KW_END
// <stmt> -> KW_RETURN      <expr>      <expr2>
// <stmt> -> <ID_assign_or_fun>
void syntax_stmt(){
    printf("stmt\n");
    if(token == KW_LOCAL){
        printf("stmt-local\n");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak :");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(!syntax_type()) errorMessage(ERR_SYNTAX, "Očekával se typ");;

        syntax_var_init();
    } 
    else if(token == ID ||(token < UNKNOWN && token > KW_WHILE)){
        printf("stmt-id-or-fun\n");
        syntax_ID_assign_or_fun();
    } 
    else if(token == KW_IF){
        printf("stmt-if\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        bottom_up();
        if(token != KW_THEN) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"then\"");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_stmts();

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == KW_ELSE) syntax_else();
        

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        if(token == KW_END) printf("Je tu slovo \"end\"\n");
    }     
    else if(token == KW_WHILE){
        printf("stmt-while\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        bottom_up();
        
        if(token != KW_DO) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"do\"");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_stmts();

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
    } 
    else if(token == KW_RETURN){
        printf("stmt-return\n");
        syntax_expr();
        syntax_expr2();
    }
    else errorMessage(ERR_SYNTAX, "Očekával se statement");
}

// <else> -> else <stmts>
// <else> -> epsilon
void syntax_else(){
    printf("else\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != KW_END) {
        syntax_stmts();  
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute)); 
    } 
    printf("befelemepeseveze\n");

}

// <ID_assign_or_fun> -> <fun_call>
// <ID_assign_or_fun> -> <ID_next>   ASSIGN      <expr>  <expr2>
void syntax_ID_assign_or_fun(){
    printf("id-or-fun\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == LBR){
        syntax_fun_call();
    }
    else if(token == COMMA){
        syntax_ID_next();        
        if(token != ASSIGN) errorMessage(ERR_SYNTAX, "Očekával se znak =");
        syntax_expr();
        syntax_expr2();
    }
    else if(token == ASSIGN){
        syntax_expr();
        syntax_expr2();

    }
    else errorMessage(ERR_SYNTAX, "Očekával se znak , nebo (");
}

// <var_init> -> ASSIGN <expr>
// <var_init> -> epsilon
void syntax_var_init(){
    printf("var_init\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ASSIGN){
        syntax_expr();
        token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
}

// <expr> -> <fun_call>
// <expr> -> term
void syntax_expr(){
    printf("expr\n");
    token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID){
        syntax_fun_call();
    }
}

// <expr2> -> COMMA <expr> <expr2>
// <expr2> -> epsilon
void syntax_expr2(){
    printf("expr2\n");
    token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_expr();
        syntax_expr2();
        token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
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

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
    
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA) syntax_ID_next();
}