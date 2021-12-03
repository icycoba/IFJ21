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
    varTableInit(&varTable);
    funcTableInit(&funcTable);
    DLL_Init(&currentList);
    DLL_Init(&assignExpr);
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    if(strInit(&attributeTemp)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    if(strInit(&currentFunc)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    if(strInit(&currentVar)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    
    strClear(&currentFunc);
    strAddString(&currentFunc, "reads");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentVar);
    strAddString(&currentVar, "string");
    addReturnParam(&funcTable, currentFunc, currentVar);

    strClear(&currentFunc);
    strAddString(&currentFunc, "readi");
    funcTableInsert(&funcTable, currentFunc);   
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);;

    strClear(&currentVar);
    strAddString(&currentVar, "integer");
    addReturnParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentFunc);
    strAddString(&currentFunc, "readn");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentVar);
    strAddString(&currentVar, "number");
    addReturnParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentFunc);
    strAddString(&currentFunc, "write");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentFunc);
    strAddString(&currentFunc, "tointeger");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentVar);
    strAddString(&currentVar, "number");
    addParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentVar);
    strAddString(&currentVar, "integer");
    addReturnParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentFunc);
    strAddString(&currentFunc, "substr");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentVar);
    strAddString(&currentVar, "string");
    addParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentVar);
    strAddString(&currentVar, "number");
    addParam(&funcTable, currentFunc, currentVar); 

    addParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentVar);
    strAddString(&currentVar, "string");
    addReturnParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentFunc);
    strAddString(&currentFunc, "ord");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentVar);
    strAddString(&currentVar, "string");
    addParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentVar);
    strAddString(&currentVar, "integer");
    addParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentVar);
    strAddString(&currentVar, "integer");
    addReturnParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentFunc);
    strAddString(&currentFunc, "chr");
    funcTableInsert(&funcTable, currentFunc);
    funcDeclared(&funcTable, currentFunc);
    funcDefined(&funcTable, currentFunc);

    strClear(&currentVar);
    strAddString(&currentVar, "integer");
    addParam(&funcTable, currentFunc, currentVar); 

    strClear(&currentVar);
    strAddString(&currentVar, "string");
    addReturnParam(&funcTable, currentFunc, currentVar); 



    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    syntax_program();

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != EOFILE) errorMessage(ERR_SYNTAX, "Chybí EOF");

    controlDefined(&funcTable);

    strFree(&currentVar);
    strFree(&currentFunc);
    strFree(&attribute);
    DLL_Dispose(&currentList);
    DLL_Dispose(&assignExpr);
    funcTableDispose(&funcTable);
    varTableDispose(&varTable);
    //printf("syntakticka analyza probehla bez problemu\n");
    return SYNTAX_OK;  
}
        
void bottom_up(){
    printf("bottom-up\n");
    Stack *s;
    s = (Stack *) malloc(sizeof(Stack));
    stack_init(s);
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    while((token >= STRING && token <= RBR) || token == LEN || token == ID || token == ZERO || token == KW_NIL || token == CONCAT){
        /*if(stack_isEmpty(s)){
            stack_push(s, token);
        }
        else{
            if(s->arr[s->top]==ID&&(token!=LBR)){
                stack_pop(s);
            }
            else if(s->arr[s->top]==RBR&&(token!=LBR)){
                stack_pop(s);
            }
            else if(s->arr[s->top]==LBR){
                stack_push(s, token);
            }
            else if(s->arr[s->top]==GT || s->arr[s->top]==GTE || s->arr[s->top]==LT || s->arr[s->top]==LTE || s->arr[s->top]==EQUAL || s->arr[s->top]==NEQUAL){
                if(token == GT ||token == GTE ||token == LT ||token == LTE ||token == EQUAL ||token == NEQUAL){
                    stack_pop(s);
                }
                else if(token == RBR){
                    stack_pop(s);
                }
                else{
                    stack_push(s, token);
                }
            }
            else if(s->arr[s->top] == CONCAT){
                   if(token == GT ||token == GTE ||token == LT ||token == LTE ||token == EQUAL ||token == NEQUAL){
                    stack_pop(s);
                }
                else if(token == RBR){
                    stack_pop(s);
                }
                else{
                    stack_push(s, token);
                }
            }
            else if(s->arr[s->top] == ADD || s->arr[s->top] == SUB){
                if(token == ADD || token == SUB || token == CONCAT){
                    stack_pop(s);
                }
                else if(token == GT || token == GTE ||token == LT ||token == LTE ||token == EQUAL ||token == NEQUAL){
                    stack_pop(s);
                }
                else if(token == RBR){
                    stack_pop(s);
                }
                else{
                    stack_push(s, token);
                }
            }
            else if(s->arr[s->top] == MUL || s->arr[s->top] == DIV || s->arr[s->top]==DIV_WHOLE){
                if(token == LEN || token == LBR || token == ID){
                    stack_push(s, token);
                }
                else{
                    stack_pop(s);
                }
            }
            else if(s->arr[s->top] == LEN){
                if(token == LBR || token == ID){
                    stack_push(s, token);
                }
                else{
                    stack_pop(s);
                }
            }
            else errorMessage(ERR_SYNTAX, "Chyba precedence");

            printf("STACK_TOP: %d", s->arr[s->top]);
        }*/
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    stack_delete(s);
    printf("bottom-up-end\n");
}

//// každá syntax funkce je jeden neterminál v gramatice ////

// <program> -> <prolog> <fun_dec_def_call> EOF
void syntax_program(){
    //printf("program\n");
    if (token != EOFILE){
        syntax_prolog();
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_fun_dec_def_call();        
    }

    //TODO if token != EOFILE => SYNTAX / SEMANTICS ERROR
}

// <prolog> -> KW_REQUIRE "ifj21"
void syntax_prolog(){
    printf("prolog\n");
    printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != STRING || strCmpConstStr(&attribute, "\"ifj21\"")) errorMessage(ERR_SYNTAX, "Klíčové slovo require musí následovat řetězec \"ifj21\"");
    //generateProlog();

    fprintf(stdout, ".IFJcode21\n");
    fprintf(stdout, "JUMP $__main__\n");
}

// <fun_dec_def_call> -> KW_GLOBAL  ID DOUBLEDOT    KW_FUNC         LBR <param_type>    RBR     <type_rtrn> <fun_dec_def_call>
// <fun_dec_def_call> -> KW_FUNC    ID LBR          <fun_params>    RBR <type_rtrn>     <stmts> KW_END      <fun_dec_def_call>
// <fun_dec_def_call> -> <fun_call> <fun_dec_def_call>
// <fun_dec_def_call> -> epsilon
void syntax_fun_dec_def_call(){
    printf("fun_dec_def_call\n");
    if (token == KW_GLOBAL){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX /*MOZNA ERR_SYNTAX TODO nejsem si jisty jaky tady bude error -xhlins01*/, "Pokus o definici non-ID");
        //je atribut jiz declared?
        if(funcTableSearch(&funcTable, attribute)){
            errorMessage(ERR_NONDEF, "Pokus o redeklaraci funkce");
        } else{
            funcTableInsert(&funcTable, attribute);
        }
        strCopyString(&currentFunc, &attribute);
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak : v deklaraci funkce");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_FUNC) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo function v deklaraci funkce");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak ( v deklaraci funkce");
        syntax_param_type();

        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ) v deklaraci funkce");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT) syntax_type_rtrn();

        funcDeclared(&funcTable, currentFunc);
        syntax_fun_dec_def_call();
    }
    else if (token == KW_FUNC){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if (token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");
        
        if (funcTableSearch(&funcTable, attribute)){
                    if (funcTableSearch(&funcTable, attribute)->defined == true) errorMessage(ERR_NONDEF, "Pokus o redefinici funkce");}
        
        else {funcTableInsert(&funcTable, attribute);}
        
        strCopyString(&currentFunc, &attribute);
        //generateLabel(&attribute);
        fprintf(stdout, "LABEL $%s\n", strGetStr(&attribute));

        // TODO sémantika
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak (");

        syntax_fun_params();
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak )");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT) syntax_type_rtrn();

        
        
        funcDefined(&funcTable, currentFunc);
        //syntax_type_rtrn();
        syntax_stmts();

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo end na konci funkce");
        scopeSub(&varTable);

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        
        syntax_fun_dec_def_call();
    }
    else if(token == ID){
        //generateMain();
        //generateCall(&attribute);
        if(!mainLabel){
            fprintf(stdout, "LABEL $__main__\n");
            fprintf(stdout, "CREATEFRAME\n");
            fprintf(stdout, "PUSHFRAME\n");
            mainLabel = true;
        }

        if(!funcTableSearch(&funcTable, attribute)) errorMessage(ERR_NONDEF, "Volání nedefinované funkce funkce");
        
        strCopyString(&currentVar, &attribute);

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_fun_call();

        syntax_fun_dec_def_call();
    }
    else{
        if(token != EOFILE) errorMessage(ERR_SYNTAX, "Očekávalo se volani, deklarace nebo definice funkce");
    }
    //else{
    //    if(token != EOFILE) errorMessage(ERR_SYNTAX, "Program neobsahuje EOF");
    //}
    
}

// <fun_call> -> ID LBR <fun_call_params> RBR
void syntax_fun_call(){
    printf("fun_call\n");
    //token = getToken(&attribute);
    if (token == LBR){
        
        syntax_fun_call_params();
        
        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se se znak ')'");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        printf("yeah\n");
        simple_print(&varTable);
        simple_print2(&funcTable);
        
        if(!DLL_Compare(&currentList, &funcTableSearch(&funcTable, currentVar)->param)) errorMessage(ERR_RETURN, "Parametry se neshoduji");
        
        DLL_Dispose(&currentList);
        DLL_Init(&currentList);
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
    }
    //else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <param_type2> -> COMMA <type> <param_type2>
// <param_type2> -> epsilon
void syntax_param_type2(){
    printf("param_type2\n");
    addParam(&funcTable, currentFunc, attribute);
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_param_type2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
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

    if(!funcTableSearch(&funcTable, currentFunc)->declared && !funcTableSearch(&funcTable, currentFunc)->defined) {addReturnParam(&funcTable, currentFunc, attribute); printf("another return param in the wall\n");}


    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
}

// <fun_params> -> ID DOUBLEDOT <type> <fun_params2>
// <fun_params> -> epsilon
void syntax_fun_params(){
    printf("fun_params\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID){
        varTableInsert(&varTable, attribute);
        strCopyString(&currentVar, &attribute);
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT){
            token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(syntax_type()){ 
                varTypeAdd(&varTable, currentVar, attribute);
                printf("yep\n");
                printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(!funcTableSearch(&funcTable, currentFunc)->declared && !funcTableSearch(&funcTable, currentFunc)->defined) {addParam(&funcTable, currentFunc, attribute); printf("another param in the wall2\n");}
                syntax_fun_params2();
            }
            else errorMessage(ERR_SYNTAX, "Očekával se typ funkce");
        }
        else errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
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
            varTableInsert(&varTable, attribute);
            strCopyString(&currentVar, &attribute);
            token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(token == DOUBLEDOT){
                token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(syntax_type()){
                    varTypeAdd(&varTable, currentVar, attribute);
                    if(!funcTableSearch(&funcTable, currentFunc)->declared && !funcTableSearch(&funcTable, currentFunc)->defined) {addParam(&funcTable, currentFunc, attribute); printf("another param in the wall3\n");}
                    syntax_fun_params2();
                    
                }
                else errorMessage(ERR_SYNTAX, "Očekával se typ funkce");
            }
            else errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
        }
        else errorMessage(ERR_SYNTAX, "Očekávalo se ID");
    }
    
}

// <fun_call_params> -> ID <fun_call_params2>
// <fun_call_params> -> epsilon
void syntax_fun_call_params(){
    printf("fun_call_params\n");
    
    simple_print2(&funcTable);
    printf("\n");    
    simple_print(&varTable);    
    printf("\n");

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    
    if(token == ID || (token >= STRING && token <= EXP)) {
        syntax_fun_call_params2();
        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        
    }
    
}

// <fun_call_params2> -> COMMA ID <fun_call_params2>
// <fun_call_params2> -> epsilon
void syntax_fun_call_params2(){
    printf("fun_call_params2\n");   
    
    if(token == ID){
        if(varTableSearch(&varTable, attribute) == NULL) errorMessage(ERR_NONDEF, "Nedefinované proměná ve volání funkce");
        DLL_InsertLast(&currentList, varTableSearch(&varTable, attribute)->key);
    }
    else if(token == STRING){
        strClear(&attribute);
        strAddString(&attribute, "string");
        DLL_InsertLast(&currentList, attribute);
    }
    else if(token == INT){
        strClear(&attribute);
        strAddString(&attribute, "integer");
        DLL_InsertLast(&currentList, attribute);
    }
    else if(token == DOUBLE || token == EXP){
        strClear(&attribute);
        strAddString(&attribute, "number");
        DLL_InsertLast(&currentList, attribute);
    }    


    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == ID || (token >= STRING && token <= EXP)) syntax_fun_call_params2();           
        
        else errorMessage(ERR_SYNTAX, "Očekávalo se ID");
        
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

// <stmt> -> KW_LOCAL   ID          DOUBLEDOT   <type>      <var_init>
// <stmt> -> KW_IF      expression  KW_THEN     <stmts>     <else>   KW_END
// <stmt> -> KW_WHILE   expression  KW_DO       <stmts>     KW_END
// <stmt> -> ID         <ID_next>   =           <expr>
// <stmt> -> <fun_call>
// <stmt> -> KW_RETURN  expression  <expr2>
void syntax_stmt(){
    printf("stmt\n");
    if(token == KW_LOCAL){
        printf("stmt-local\n");
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");
        
        if(varTableSearch(&varTable, attribute)){ 
            if(varTableSearch(&varTable, attribute)->scope == 0) errorMessage(ERR_NONDEF, "Pokus o redeklaraci proměnné");
        }
        
        else varTableInsert(&varTable, attribute);
        
        strCopyString(&currentVar, &attribute);

        //generateVar(&attribute);

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak :");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(!syntax_type()) errorMessage(ERR_SYNTAX, "Očekával se typ");
        varTypeAdd(&varTable, currentVar, attribute);
        
        syntax_var_init();
    }     
    else if(token == KW_IF){
        printf("stmt-if\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        scopeAdd(&varTable);
        bottom_up();
        if(token != KW_THEN) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"then\"");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        

        syntax_stmts();
        
        if(token == KW_ELSE) syntax_else();
        

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        scopeSub(&varTable);

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }     
    else if(token == KW_WHILE){
        printf("stmt-while\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        scopeAdd(&varTable);
        bottom_up();
        
        if(token != KW_DO) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"do\"");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_stmts();

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        scopeSub(&varTable);

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    } 
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        printf("stmt-id-or-fun\n");
        strCopyString(&attributeTemp, &attribute);

        strCopyString(&currentVar, &attribute);
        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == F_WRITE){  

            token = getToken(&attribute);
            if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak '('");
            token = getToken(&attribute);
            while((token >= STRING && token <= EXP) || token == ID){
                token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(token == RBR) break;
                else if (token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak ,");
                token = getToken(&attribute);  printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(!(token == ID || (token >= STRING && token <= EXP))) errorMessage(ERR_SYNTAX, "Očekával se ID, int, string nebo number");
            }
            if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ')'");
            token = getToken(&attribute);
        }
        else if(funcTableSearch(&funcTable, currentVar)){
            generateCall(&attributeTemp);
            syntax_fun_call();
        }
        else if(varTableSearch(&varTable, currentVar)){
            DLL_InsertLast(&currentList, varTableSearch(&varTable, currentVar)->type);

            strCopyString(&currentVar, &attribute);

            token = getToken(&attribute);
            if(token == COMMA) syntax_ID_next(); 

            if(token != ASSIGN) errorMessage(ERR_SYNTAX, "Očekával se znak =");
            syntax_expr();

            if(assignExpr.firstElement != NULL){
                if(!DLL_Compare(&assignExpr, &currentList)) errorMessage(ERR_ASSIGN, "Prava a leva strana nema stejne typy nebo pocet parametru");
                DLL_Dispose(&assignExpr);
                DLL_Init(&assignExpr);
            }
            if(currentList.firstElement != NULL){
                DLL_Dispose(&currentList);
                DLL_Init(&currentList);
            }
        }
        
        else errorMessage(ERR_NONDEF, "Nedefinovana proměnná nebo funkce");
    } 
    else if(token == KW_RETURN){
        printf("stmt-return\n");
        token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if ((token >= STRING && token <= EXP) || token == LEN || token == ID){
            bottom_up();
            syntax_expr2();
        }
    }
    else errorMessage(ERR_SYNTAX, "Očekával se statement");
}

// <else> -> else <stmts>
// <else> -> epsilon
void syntax_else(){
    printf("else\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != KW_END) {
        printf("befelemepeseveze2\n");
        syntax_stmts();  printf("befelemepeseveze1\n");
    } 
    printf("befelemepeseveze\n");

}


// <var_init> -> ASSIGN <init>
// <var_init> -> epsilon
void syntax_var_init(){
    printf("var_init\n");
    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ASSIGN){
        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_init();
    }
}

// <init> -> expression
// <init> -> <fun_call>
void syntax_init(){
    printf("init\n");
    if(token == LEN || token == LBR || token == ZERO || token == KW_NIL || (token >= STRING && token <= EXP)){
        if(token == LEN || token == LBR || token == INT){
            strClear(&attribute);
            strAddString(&attribute, "integer");
            if(strCmpString(&attribute, &varTableSearch(&varTable, currentVar)->type)) errorMessage(ERR_ASSIGN, "Špatný typ při inicializaci");
        }
        else if(token == STRING){
            strClear(&attribute);
            strAddString(&attribute, "string");
            if(strCmpString(&attribute, &varTableSearch(&varTable, currentVar)->type)) errorMessage(ERR_ASSIGN, "Špatný typ při inicializaci");
        }
        else if(token == DOUBLE || token == EXP){
            strClear(&attribute);
            strAddString(&attribute, "number");
            if(strCmpString(&attribute, &varTableSearch(&varTable, currentVar)->type)) errorMessage(ERR_ASSIGN, "Špatný typ při inicializaci");
        }

        bottom_up();
    }
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        
        //token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(funcTableSearch(&funcTable, attribute)){
            if(DLL_length(&funcTableSearch(&funcTable, attribute)->returnParam) != 1) errorMessage(ERR_ASSIGN, "Funkce vraci spatny pocet parametru");

            if(strCmpString(&varTableSearch(&varTable, currentVar)->type, &funcTableSearch(&funcTable, attribute)->returnParam.firstElement->data))
                errorMessage(ERR_ASSIGN, "Funkce vraci spatny typ pri inicializaci funkce");

            token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            syntax_fun_call();
        }
        else if(varTableSearch(&varTable, attribute)){
            if(strCmpString(&varTableSearch(&varTable, currentVar)->type, &varTableSearch(&varTable, attribute)->type))
                errorMessage(ERR_ASSIGN, "Promenna ma spatny typ pri inicializaci funkce");
            bottom_up();
        }
        
        else errorMessage(ERR_NONDEF, "Nedefinovana funkce nebo promenna");

    }
    else errorMessage(ERR_SYNTAX, "Očekával se vyraz nebo volani funkce");
}

 // <expr> -> <fun_call> 
 // <expr> -> expression <expr2>
void syntax_expr(){
    printf("expr\n");
    token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));   
    if(token == LEN || token == RBR || token == LBR || token == ZERO || token == KW_NIL || (token >= STRING && token <= EXP)){
        if(token == LEN || token == LBR || token == INT){
            strClear(&attribute);
            strAddString(&attribute, "integer");
            DLL_InsertLast(&assignExpr, attribute);
        }
        else if(token == STRING){
            strClear(&attribute);
            strAddString(&attribute, "string");
            DLL_InsertLast(&assignExpr, attribute);
        }
        else if(token == DOUBLE || token == EXP){
            strClear(&attribute);
            strAddString(&attribute, "number");
            DLL_InsertLast(&assignExpr, attribute);
        }
        bottom_up();
        syntax_expr2();
    }
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        //token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        strCopyString(&currentVar, &attribute);
        if(funcTableSearch(&funcTable, currentVar)){
            
            if(!DLL_Compare(&currentList, &funcTableSearch(&funcTable, currentVar)->returnParam)) 
                errorMessage(ERR_ASSIGN, "Funkce vraci hodnoty ktere se neshoduji s identifikatory");

            DLL_Dispose(&currentList);
            DLL_Init(&currentList);

            token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            syntax_fun_call();
        }
        else if(varTableSearch(&varTable, currentVar)){
            DLL_InsertLast(&assignExpr, varTableSearch(&varTable, currentVar)->type);
            bottom_up();
            syntax_expr2();
        }       
        
        else errorMessage(ERR_SYNTAX, "Očekával se vyraz nebo volani funkce");    
    }
}

// <expr2> -> COMMA expression <expr2>
// <expr2> -> epsilon
void syntax_expr2(){
    printf("expr2\n");    
    if(token == COMMA){
        token = getToken(&attribute);printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if (!((token >= STRING && token <= NEQUAL) || token == LEN)) errorMessage(ERR_SYNTAX, "Očekával se vyraz");  
        if(token == LEN || token == LBR || token == INT){
            strClear(&attribute);
            strAddString(&attribute, "integer");
            DLL_InsertLast(&assignExpr, attribute);
        }
        else if(token == STRING){
            strClear(&attribute);
            strAddString(&attribute, "string");
            DLL_InsertLast(&assignExpr, attribute);
        }
        else if(token == DOUBLE || token == EXP){
            strClear(&attribute);
            strAddString(&attribute, "number");
            DLL_InsertLast(&assignExpr, attribute);
        }
        bottom_up();
        syntax_expr2();
    }
}

// <type> -> KW_STR
// <type> -> KW_INT
// <type> -> KW_NUM
bool syntax_type(){
    printf("type\n");
    if(token == KW_STR || token == KW_INT || token == KW_NUM || token == KW_NIL) return true;
    else return false;
}

// <ID_next> -> COMMA ID <ID_next>
// <ID_next> -> epsilon
void syntax_ID_next(){
    printf("ID_next\n");

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
    
    if(!varTableSearch(&varTable, attribute)) errorMessage(ERR_NONDEF, "Nedefinovana promenna v assign");
    DLL_InsertLast(&currentList, varTableSearch(&varTable, attribute)->type);

    token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA) syntax_ID_next();
}

// Inicializace zásobníku
void stack_init(Stack *s){
    if(s==NULL){
        errorMessage(ERR_INTERNAL, "Chyba inicializace zásobníku");
    }
    s->top = -1;
}

// Je zásobník plný?
int stack_isFull(const Stack *s){
    return (s->top == STACK_MAX_SIZE-1);
}

// Je záspbník prázdný?
int stack_isEmpty(const Stack *s){
    //printf("Vidím ťa");
    return (s->top == -1);
}

// Uložení tokenu na zásobník
void stack_push(Stack *s, int token){
    //printf("Vidím ťa2");
    if(!stack_isFull(s)){
        s->top++;
        s->arr[s->top]=token;
    }
    else{
        errorMessage(ERR_INTERNAL, "Chyba vložení tokenu na zásobník");
    }
}

// Odstranění tokenu z vrcholu zásobníku
void stack_pop(Stack *s){
    printf("Nevidím ťa");
    if(stack_isEmpty(s)) return;
    else s->top--;
}

// Vracení znaku na vrcholu zásobníku
void stack_top(const Stack *s, int *tokenPtr){
    if(stack_isEmpty(s)){
        errorMessage(ERR_INTERNAL, "Nenalezen vrchol zásobníku");
    }
    else *tokenPtr = s->arr[(s->top)];
}

// Odstranění zásobníku
void stack_delete(Stack *s){
    while(!stack_isEmpty(s)){
        stack_pop(s);
    }
}