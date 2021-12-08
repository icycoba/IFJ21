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
bool mainLabel = false;


int parser(){
    varTableInit(&varTable);
    funcTableInit(&funcTable);
    DLL_Init(&currentList);
    DLL_Init(&assignExpr);
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    if(strInit(&attributeTemp)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    if(strInit(&funcName)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    if(strInit(&currentFunc)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    if(strInit(&currentVar)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");
    s = (Stack *) malloc(sizeof(Stack));    
    stack_init(s);
    exprEnd = false;
    skip = false;
    
    
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



    token = getToken(&attribute);
    syntax_program();

    token = getToken(&attribute);
    if (token != EOFILE) errorMessage(ERR_SYNTAX, "Chybí EOF");

    controlDefined(&funcTable);

    free(s);
    strFree(&currentVar);
    strFree(&currentFunc);
    strFree(&attribute);
    DLL_Dispose(&currentList);
    DLL_Dispose(&assignExpr);
    funcTableDispose(&funcTable);
    varTableDispose(&varTable);
    return SYNTAX_OK;  
}
        
void bottom_up(){    
    
    if(stack_highest(s) == -1){            
        if(exprEnd){
            fprintf(stderr, "vyraz zpracovan uspesne\n");
            return;
        }
        else if(token == RBR)
            errorMessage(ERR_SYNTAX, "Chyba precedence1");
        else{
            stack_handle(s);
            stack_push(s, token);
        }
    }
    else if(s->arr[stack_highest(s)] == ID || 
            (s->arr[stack_highest(s)] >= STRING && s->arr[stack_highest(s)] <= EXP) ||  
            s->arr[stack_highest(s)] == ZERO || s->arr[stack_highest(s)] == KW_NIL){            
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == LBR || token == LEN || (token >= STRING && token <= EXP) || token == ZERO || token == KW_NIL)
            errorMessage(ERR_SYNTAX, "Chyba precedence2");

        else if(token == ID)
            exprEnd = true;
        else{
            stack_rules(s);
            skip = true;
        }            
    }
    else if(s->arr[stack_highest(s)] == RBR){
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == LEN || token == LBR || (token >= STRING && token <= EXP) || token == KW_NIL || token == ZERO)
            errorMessage(ERR_SYNTAX, "Chyba precedence3");
        else if(token == ID)
            exprEnd = true;
        else{
            stack_rules(s);
            skip = true;
        }
        
    }
    else if(s->arr[stack_highest(s)]==LBR){
        if(exprEnd)
            errorMessage(ERR_SYNTAX, "Chyba precedence4");
        else if(token == RBR){
            if(s->arr[s->top - 1] == LBR && s->arr[s->top] == E){
                s->top -= 2;
                s->arr[s->top] = E;
            }
        }
        else{
            stack_handle(s);
            stack_push(s, token);
        }
    }
    else if(s->arr[stack_highest(s)]==GT || s->arr[stack_highest(s)]==GTE || s->arr[stack_highest(s)]==LT || 
            s->arr[stack_highest(s)]==LTE || s->arr[stack_highest(s)]==EQUAL || s->arr[stack_highest(s)]==NEQUAL){        
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == GT || token == GTE || token == LT || token == LTE 
            || token == EQUAL || token == NEQUAL || token == RBR || exprEnd){
            stack_rules(s);
            skip = true;
        }
        else{
            stack_handle(s);
            stack_push(s, token);
        }
    }
    else if(s->arr[stack_highest(s)] == CONCAT){
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == GT || token == GTE || token == LT || token == LTE || token == EQUAL || token == NEQUAL || token == RBR || exprEnd){
            stack_rules(s);
            skip = true;
        }
        else{
            stack_handle(s);
            stack_push(s, token);
        }
    }
    else if(s->arr[stack_highest(s)] == ADD || s->arr[stack_highest(s)] == SUB){
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == ADD || token == SUB || token == CONCAT || token == RBR || token == GT ||
            token == GTE || token == LT || token == LTE || token == EQUAL || token == NEQUAL || exprEnd){
            stack_rules(s);
            skip = true;
        }
        else{
            stack_handle(s);
            stack_push(s, token);
        }
    }
    else if(s->arr[stack_highest(s)] == MUL || s->arr[stack_highest(s)] == DIV || s->arr[stack_highest(s)] == DIV_WHOLE){
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == LEN || token == LBR || token == ID || (token >= STRING && token <= EXP) || token == ZERO || token == KW_NIL){
            stack_handle(s);
            stack_push(s, token);
        }
        else{
            stack_rules(s);
            skip = true;
        }
    }
    else if(s->arr[stack_highest(s)] == LEN){
        if(exprEnd){
            stack_rules(s);
            skip = true;
        }
        else if(token == LBR || token == ID || (token >= STRING && token <= EXP) || token == ZERO || token == KW_NIL){
            stack_handle(s);
            stack_push(s, token);
        }
        else if(token == LEN)
            errorMessage(ERR_SYNTAX, "Chyba precedence5");
        else{
            stack_rules(s);
            skip = true;
        }
    }
    else {
    errorMessage(ERR_SYNTAX, "Chyba v kodu precedence, tady by se to ani nemelo dostat");}

    for(int i = 0; i < s->top + 1; i++){
        fprintf(stderr, "%d  ", s->arr[s->top - i]);
    }
    fprintf(stderr, " start]\n");
    

    //if((exprOutcome >= STRING && exprOutcome <= EXP) || exprOutcome == ID){
    //    if((token >= STRING && token <= EXP) || token == ID) break;            
    //}
    //exprOutcome = token;
    if(!exprEnd && !skip){
        if(token >= STRING && token <= ID){
            fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
            //fprintf(stdout, "PUSHS string@test\n");
            if(token == STRING){
                fprintf(stdout, "PUSHS string@%s\n", strGetStr(&attribute));
            }else if(token == INT){
                fprintf(stdout, "PUSHS int@%s\n", strGetStr(&attribute));
            }else if(token == DOUBLE || token == EXP){
                fprintf(stdout, "PUSHS float@%a\n", atof(strGetStr(&attribute)));
            }else if(token == ID){
                fprintf(stdout, "PUSHS LF@%s\n", strGetStr(&attribute));
            }
        }
        if((token >= STRING && token <= ID) || token == ZERO){
            fprintf(stderr, "[%s]\n", printState(exprOutcome));
            if(exprOutcome == KW_NIL){
                if(token == STRING){
                    if(s->top - 2 >= 0 && s->arr[s->top - 2] == LEN) exprOutcome = INT;
                    else exprOutcome = STRING;
                }
                else if(token == INT || token == ZERO){
                    exprOutcome = INT;
                }
                else if(token == DOUBLE || token == EXP){
                    exprOutcome = DOUBLE;
                }
                else if(token == ID){
                    if(!strCmpConstStr(&varTableSearch(&varTable, attribute)->type, "integer")){
                        exprOutcome = INT;
                    }
                    else if(!strCmpConstStr(&varTableSearch(&varTable, attribute)->type, "string")){
                        if(s->top - 2 >= 0 && s->arr[s->top - 2] == LEN) exprOutcome = INT;
                        else exprOutcome = STRING;
                    }
                    else if(!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type,"number")){
                        exprOutcome = DOUBLE;
                    }
                }                
            }
            else{
                if(token == STRING){
                    if(!stack_isEmpty(s) && s->arr[s->top - 2] == LEN){
                        fprintf(stderr, "[%d]\n", s->arr[s->top]);
                         if(exprOutcome != INT && exprOutcome != DOUBLE) errorMessage(ERR_TYPE_CMP, "Typ nesedí1");
                    }
                    else if(exprOutcome != STRING) errorMessage(ERR_TYPE_CMP, "Typ nesedí2");
                }
                else if(token == INT || token == ZERO){
                    if(exprOutcome != INT && exprOutcome != DOUBLE) errorMessage(ERR_TYPE_CMP, "Typ nesedí3");
                }
                else if(token == DOUBLE || token == EXP){
                    if(exprOutcome != DOUBLE) errorMessage(ERR_TYPE_CMP, "Typ nesedí4");
                }

            }

        }


        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LEN && !(token >= STRING && token <= RBR) && token != ID && token != ZERO && token != KW_NIL && token != CONCAT)
            exprEnd = true;
        if(token == GT || token == GTE || token == LT || token == LTE || token == EQUAL || token == NEQUAL)
            exprOutcome = KW_NIL;
    }
    skip = false;
    
    
    fprintf(stderr, "bottom-up-end\n");
    bottom_up();
}

//// každá syntax funkce je jeden neterminál v gramatice ////

// <program> -> <prolog> <fun_dec_def_call> EOF
void syntax_program(){
    if (token != EOFILE){
        syntax_prolog();
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_fun_dec_def_call();        
    } else{
        errorMessage(ERR_SYNTAX, "Input file je prázdný / neočekávaný EOF");
    }

    //TODO if token != EOFILE => SYNTAX / SEMANTICS ERROR
}

// <prolog> -> KW_REQUIRE "ifj21"
void syntax_prolog(){
    fprintf(stderr, "prolog\n");
    fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != STRING || strCmpConstStr(&attribute, "ifj21")) errorMessage(ERR_SEM_OTHER, "Klíčové slovo require musí následovat řetězec \"ifj21\"");
    //generateProlog();

    fprintf(stdout, ".IFJcode21\n");
    fprintf(stdout, "JUMP $__main__\n");
    generatePredefined();
}

// <fun_dec_def_call> -> KW_GLOBAL  ID DOUBLEDOT    KW_FUNC         LBR <param_type>    RBR     <type_rtrn> <fun_dec_def_call>
// <fun_dec_def_call> -> KW_FUNC    ID LBR          <fun_params>    RBR <type_rtrn>     <stmts> KW_END      <fun_dec_def_call>
// <fun_dec_def_call> -> <fun_call> <fun_dec_def_call>
// <fun_dec_def_call> -> epsilon
void syntax_fun_dec_def_call(){
    fprintf(stderr, "fun_dec_def_call\n");
    if (token == KW_GLOBAL){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX /*MOZNA ERR_SYNTAX TODO nejsem si jisty jaky tady bude error -xhlins01*/, "Pokus o definici non-ID");
        //je atribut jiz declared?
        if(funcTableSearch(&funcTable, attribute)){
            errorMessage(ERR_NONDEF, "Pokus o redeklaraci funkce");
        } else{
            funcTableInsert(&funcTable, attribute);
        }
        strCopyString(&currentFunc, &attribute);
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak : v deklaraci funkce");

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_FUNC) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo function v deklaraci funkce");

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak ( v deklaraci funkce");
        syntax_param_type();

        //token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ) v deklaraci funkce");
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT) syntax_type_rtrn();

        funcDeclared(&funcTable, currentFunc);
        syntax_fun_dec_def_call();
    }
    else if (token == KW_FUNC){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if (token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");
        
        if (funcTableSearch(&funcTable, attribute)){
                    if (funcTableSearch(&funcTable, attribute)->defined == true) errorMessage(ERR_NONDEF, "Pokus o redefinici funkce");}
        
        else {funcTableInsert(&funcTable, attribute);}
        
        strCopyString(&currentFunc, &attribute);
        //generateLabel(&attribute);
        fprintf(stdout, "LABEL $%s\n", strGetStr(&attribute));
        fprintf(stdout, "PUSHFRAME\n");

        // TODO sémantika
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak (");

        syntax_fun_params();
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak )");

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT) syntax_type_rtrn();
       
        
        
        funcDefined(&funcTable, currentFunc);
        //syntax_type_rtrn();
        syntax_stmts();

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo end na konci funkce");
        fprintf(stdout, "POPFRAME\n");
        fprintf(stdout, "RETURN\n\n");
        scopeSub(&varTable);

        if(DLL_length(&funcTableSearch(&funcTable, currentFunc)->returnParam) != 0 && funcTableSearch(&funcTable, currentFunc)->returned == false)
            errorMessage(ERR_RETURN, "Chybí return ve funkci která má něco vracet");
        
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        
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
        strCopyString(&funcName, &attribute);
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        
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
    fprintf(stderr, "fun_call\n");
    //token = getToken(&attribute);
    if (token == LBR){
        
        syntax_fun_call_params();
        fprintf(stdout, "CREATEFRAME\n");
        for(int i = 1; i <= DLL_length(&funcTableSearch(&funcTable, funcName)->param); i++){
            fprintf(stdout, "DEFVAR TF@%%inval%%%d\n", i);
        }
        for(int i = 1; i <= DLL_length(&funcTableSearch(&funcTable, funcName)->returnParam); i++){
            fprintf(stdout, "DEFVAR TF@%%retval%%%d\n", i);
        }
        fprintf(stdout, "CALL $%s\n", strGetStr(&funcName));
        //token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se se znak ')'");
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));

        fprintf(stderr, "%d, %d",DLL_length(&currentList), DLL_length(&funcTableSearch(&funcTable, currentVar)->param ));
        fprintf(stderr, "%s\n\n", strGetStr(&funcTableSearch(&funcTable, currentVar)->key));
        if(!DLL_Compare(&currentList, &funcTableSearch(&funcTable, currentVar)->param)) errorMessage(ERR_RETURN, "Parametry se neshoduji");
        
        DLL_Dispose(&currentList);
        DLL_Init(&currentList);
        
    }
    else errorMessage(ERR_SYNTAX, "Očekával se znak '('");
}

// <param_type> -> <type> <param_type2>
// <param_type> -> epsilon
void syntax_param_type(){
    fprintf(stderr, "param_type\n");
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(syntax_type()){
        syntax_param_type2();
    }
    //else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <param_type2> -> COMMA <type> <param_type2>
// <param_type2> -> epsilon
void syntax_param_type2(){
    fprintf(stderr, "param_type2\n");
    addParam(&funcTable, currentFunc, attribute);
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_param_type2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
    //else errorMessage(ERR_SYNTAX, "Očekával se znak  ','");
}

// <type_rtrn> -> DOUBLEDOT <type> <type_rtrn2>
// <type_rtrn> -> epsilon
void syntax_type_rtrn(){
    fprintf(stderr, "type_rtrn\n");
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(syntax_type()) syntax_type_rtrn2();
    else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <type_rtrn2> -> COMMA <type> <type_rtrn2>
// <type_rtrn2> -> epsilon
void syntax_type_rtrn2(){
    fprintf(stderr, "type_rtrn2\n");  
    if(!funcTableSearch(&funcTable, currentFunc)->declared && !funcTableSearch(&funcTable, currentFunc)->defined) addReturnParam(&funcTable, currentFunc, attribute);


    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
}

// <fun_params> -> ID DOUBLEDOT <type> <fun_params2>
// <fun_params> -> epsilon
void syntax_fun_params(){
    fprintf(stderr, "fun_params\n");
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    strCopyString(&attributeTemp, &attribute);
    if(token == ID){
        varTableInsert(&varTable, attribute);
        strCopyString(&currentVar, &attribute);
        fprintf(stdout,"DEFVAR TF@%%%s\n",  strGetStr(&attribute));
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT){
            token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(syntax_type()){ 
                varTypeAdd(&varTable, currentVar, attribute);
                fprintf(stdout, "DEFVAR TF@%%%s\n", strGetStr(&attribute));
                fprintf(stdout,"POPS TF@%%%s\n",  strGetStr(&attribute));
                fprintf(stdout,"TYPE TF@%%%s TF@%%%s\n",  strGetStr(&attributeTemp), strGetStr(&attribute));
                fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(!funcTableSearch(&funcTable, currentFunc)->declared && !funcTableSearch(&funcTable, currentFunc)->defined) addParam(&funcTable, currentFunc, attribute);
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
    fprintf(stderr, "fun_params2\n");
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        strCopyString(&attributeTemp, &attribute);
        if(token == ID){   
            varTableInsert(&varTable, attribute);
            strCopyString(&currentVar, &attribute);
            fprintf(stdout,"DEFVAR TF@%%%s\n",  strGetStr(&attribute));
            token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(token == DOUBLEDOT){
                token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(syntax_type()){
                    varTypeAdd(&varTable, currentVar, attribute);
                    if(!funcTableSearch(&funcTable, currentFunc)->declared && !funcTableSearch(&funcTable, currentFunc)->defined) addParam(&funcTable, currentFunc, attribute);
                    fprintf(stdout, "DEFVAR TF@%%%s\n", strGetStr(&attribute));
                    fprintf(stdout,"POPS TF@%%%s\n",  strGetStr(&attribute));
                    fprintf(stdout,"TYPE TF@%%%s TF@%%%s\n",  strGetStr(&attributeTemp), strGetStr(&attribute));
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
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    
    if(token == ID || (token >= STRING && token <= EXP)) {
        fprintf(stdout, "DEFVAR TF@%%%s\n", strGetStr(&attribute));
        syntax_fun_call_params2();
        //token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        
    }
    
}

// <fun_call_params2> -> COMMA ID <fun_call_params2>
// <fun_call_params2> -> epsilon
void syntax_fun_call_params2(){
    fprintf(stderr, "fun_call_params2\n");   
    
    if(token == ID){
        if(varTableSearch(&varTable, attribute) == NULL) errorMessage(ERR_NONDEF, "Nedefinované proměná ve volání funkce");
        DLL_InsertLast(&currentList, varTableSearch(&varTable, attribute)->type);
    }
    else if(token == STRING){
        strClear(&attributeTemp);
        strAddString(&attributeTemp, "string");
        DLL_InsertLast(&currentList, attributeTemp);
    }
    else if(token == INT){
        strClear(&attributeTemp);
        strAddString(&attributeTemp, "integer");
        DLL_InsertLast(&currentList, attributeTemp);
    }
    else if(token == DOUBLE || token == EXP){
        strClear(&attributeTemp);
        strAddString(&attributeTemp, "number");
        DLL_InsertLast(&currentList, attributeTemp);
    }    


    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == ID || (token >= STRING && token <= EXP)) {
            fprintf(stdout,"DEFVAR TF%%%s\n",  strGetStr(&attribute));
            syntax_fun_call_params2();
        }
        else errorMessage(ERR_SYNTAX, "Očekávalo se ID");
        
    }
    
}

// <stmts> -> <stmt> <stmts>
// <stmts> -> epsilon
void syntax_stmts(){
    fprintf(stderr, "stmts\n");
    //token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
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
    fprintf(stderr, "stmt\n");
    if(token == KW_LOCAL){
        fprintf(stderr, "stmt-local\n");
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");
        if(funcTableSearch(&funcTable, attribute)) errorMessage(ERR_NONDEF, "Toto jmeno uz ma nejaka funkce");
        
        if(varTableSearch(&varTable, attribute)){ 
            if(varTableSearch(&varTable, attribute)->scope == 0) errorMessage(ERR_NONDEF, "Pokus o redeklaraci proměnné");
        }
        
        else varTableInsert(&varTable, attribute);
        
        strCopyString(&currentVar, &attribute);

        //generateVar(&attribute);

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak :");

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(!syntax_type()) errorMessage(ERR_SYNTAX, "Očekával se typ");
        varTypeAdd(&varTable, currentVar, attribute);
        fprintf(stdout, "DEFVAR LF@%s\n", strGetStr(&currentVar));
        if(!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "integer")){
            //fprintf(stdout, "MOVE LF@%s int@\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "number")){
            //fprintf(stdout, "MOVE LF@%s float@\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "string")){
            //fprintf(stdout, "MOVE LF@%s string@\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "nil")){
            fprintf(stdout, "MOVE LF@%s nil@nil\n", strGetStr(&currentVar));     
        }
        syntax_var_init();
    }     
    else if(token == KW_IF){
        fprintf(stderr, "stmt-if\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        scopeAdd(&varTable);

        fprintf(stdout, "LABEL $%s\n", strGetStr(&attribute));
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        exprOutcome = KW_NIL;
        bottom_up();
        exprEnd = false;
        stack_delete(s);
        if(token != KW_THEN) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"then\"");

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        

        syntax_stmts();
        
        if(token == KW_ELSE) syntax_else();
        

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        scopeSub(&varTable);

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    }     
    else if(token == KW_WHILE){
        fprintf(stderr, "stmt-while\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        fprintf(stdout, "LABEL $%s", strGetStr(&attribute));
        scopeAdd(&varTable);
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        exprOutcome = KW_NIL;
        bottom_up();

        fprintf(stdout, "JUMPIFEQ end LF@%%%s %%%s\n", strGetStr(&currentVar), strGetStr(&attributeTemp));
        exprEnd = false;
        stack_delete(s);
        if(token != KW_DO) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"do\"");

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_stmts();
        fprintf(stdout, "JUMP $while\n");
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        scopeSub(&varTable);

        fprintf(stdout, "LABEL %%%s\n", strGetStr(&attribute));

        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    } 
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        fprintf(stderr, "stmt-id-or-fun\n");
        strCopyString(&attributeTemp, &attribute);

        strCopyString(&currentVar, &attribute);
        //token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == F_WRITE){  

            token = getToken(&attribute); fprintf(stderr,"%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak '('");
            token = getToken(&attribute); fprintf(stderr,"%-15s |%s\n", printState(token), strGetStr(&attribute));
            
            while((token >= STRING && token <= EXP) || token == ID || token == KW_NIL){
                if(token == ID){
                    if(varTableSearch(&varTable, attribute) == NULL)
                        errorMessage(ERR_NONDEF, "Nedefinovana promenna ve funkci write");
                }         
            
                
                if(token == STRING){
                    fprintf(stdout, "WRITE string@%s\n", strGetStr(&attribute));
                } else if(token == INT){
                    fprintf(stdout, "WRITE int@%s\n", strGetStr(&attribute));
                } else if(token == DOUBLE || token == EXP){
                    fprintf(stdout, "WRITE float@%a\n", atof(strGetStr(&attribute)));
                } else if(token == ID){
                    if(varTableSearch(&varTable, attribute)){
                        fprintf(stdout, "WRITE %s\n", strGetStr(&attribute));
                    } else{
                        errorMessage(ERR_NONDEF, "Pokus o WRITE nedefinované proměnné.");
                    }
                }
                token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(token == RBR) break;
                else if (token != COMMA) errorMessage(ERR_SYNTAX, "Očekával se znak ,");
                
                token = getToken(&attribute);  fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(!(token == ID || (token >= STRING && token <= EXP)|| token == KW_NIL)) errorMessage(ERR_SYNTAX, "Očekával se ID, int, string nebo number");
            }
            if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ')'");
            token = getToken(&attribute);
        }
        else if(funcTableSearch(&funcTable, currentVar)){
            strCopyString(&funcName, &attribute);
            token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
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
                fprintf(stderr, "%d, %d",DLL_length(&currentList), DLL_length(&assignExpr));
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
        fprintf(stderr, "stmt-return\n");
        token = getToken(&attribute);fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if ((token >= STRING && token <= EXP) || token == LEN || token == ID){
            if(token == LEN || token == LBR || token == INT){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "integer");
            DLL_InsertLast(&assignExpr, attributeTemp);
            }
            else if(token == STRING){
                strClear(&attributeTemp);
                strAddString(&attributeTemp, "string");
                DLL_InsertLast(&assignExpr, attributeTemp);
            }
            else if(token == DOUBLE || token == EXP){
                strClear(&attributeTemp);
                strAddString(&attributeTemp, "number");
                DLL_InsertLast(&assignExpr, attributeTemp);
            }
            else if(token == ID){ 
                if(!varTableSearch(&varTable, attribute)) errorMessage(ERR_RETURN, "Nedefinovaná proměnná při return funkce");              
                DLL_InsertLast(&assignExpr, varTableSearch(&varTable, attribute)->type);         
            }
            exprOutcome = KW_NIL;
            bottom_up();
            exprEnd = false;
            stack_delete(s);
            syntax_expr2();
        }
        
        if(DLL_length(&assignExpr) != DLL_length(&funcTableSearch(&funcTable, currentFunc)->returnParam)) errorMessage(ERR_RETURN, "Špatný počet návratových parametrů");

        if(assignExpr.firstElement != NULL){
            if(!DLL_Compare(&assignExpr, &funcTableSearch(&funcTable, currentFunc)->returnParam)) errorMessage(ERR_ASSIGN, "Špatný typ návratových parametrů");
            DLL_Dispose(&assignExpr);
            DLL_Init(&assignExpr);
        }
        funcTableSearch(&funcTable, currentFunc)->returned = true;

    }
    else errorMessage(ERR_SYNTAX, "Očekával se statement");
}

// <else> -> else <stmts>
// <else> -> epsilon
void syntax_else(){
    fprintf(stderr, "else\n");
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != KW_END) {
        syntax_stmts();
    }
}


// <var_init> -> ASSIGN <init>
// <var_init> -> epsilon
void syntax_var_init(){
    fprintf(stderr, "var_init\n");
    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ASSIGN){
        token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_init();
    }
}

// <init> -> expression
// <init> -> <fun_call>
void syntax_init(){
    fprintf(stderr, "init\n");
    if(token == LEN || token == LBR || token == ZERO || token == KW_NIL || (token >= STRING && token <= EXP)){
        if(token == LEN || token == LBR || token == INT){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "integer");
            if(strCmpString(&attributeTemp, &varTableSearch(&varTable, currentVar)->type)) errorMessage(ERR_ASSIGN, "Špatný typ při inicializaci");
        }
        else if(token == STRING){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "string");
            if(strCmpString(&attributeTemp, &varTableSearch(&varTable, currentVar)->type)) errorMessage(ERR_ASSIGN, "Špatný typ při inicializaci");
        }
        else if(token == DOUBLE || token == EXP){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "number");
            if(strCmpString(&attributeTemp, &varTableSearch(&varTable, currentVar)->type)) errorMessage(ERR_ASSIGN, "Špatný typ při inicializaci");
        }

        //TODO runtime errory
        
        

        
        exprOutcome = KW_NIL;
        bottom_up();
        exprEnd = false;
        stack_delete(s);

        if(!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "integer")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "number")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "string")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "nil")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        }
    }
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        //token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(funcTableSearch(&funcTable, attribute)){
            if(DLL_length(&funcTableSearch(&funcTable, attribute)->returnParam) != 1) errorMessage(ERR_ASSIGN, "Funkce vraci spatny pocet parametru");

            if(strCmpString(&varTableSearch(&varTable, currentVar)->type, &funcTableSearch(&funcTable, attribute)->returnParam.firstElement->data)
                &&  (strCmpConstStr(&varTableSearch(&varTable, currentVar)->type,"number") || strCmpConstStr(&funcTableSearch(&funcTable, attribute)->returnParam.firstElement->data, "integer")))
                errorMessage(ERR_ASSIGN, "Funkce vraci spatny typ pri inicializaci funkce");

            strCopyString(&currentVar, &attribute);
            strCopyString(&funcName, &attribute);
            token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
            syntax_fun_call();
            if(DLL_length(&funcTableSearch(&funcTable, funcName)->returnParam) > 0)
                fprintf(stdout, "MOVE LF@%s LF@%%retval%%1\n", strGetStr(&currentVar));
        }
        else if(varTableSearch(&varTable, attribute)){
            if(strCmpString(&varTableSearch(&varTable, currentVar)->type, &varTableSearch(&varTable, attribute)->type))
                errorMessage(ERR_ASSIGN, "Promenna ma spatny typ pri inicializaci funkce");
            fprintf(stdout, "MOVE LF@%s kakao%s\n", strGetStr(&currentVar), strGetStr(&attribute)); //TODO kakao -> zjisteni typu attributu
            exprOutcome = KW_NIL;
            bottom_up();
            exprEnd = false;
            stack_delete(s);
        }
        
        else errorMessage(ERR_NONDEF, "Nedefinovana funkce nebo promenna");

    }
    else errorMessage(ERR_SYNTAX, "Očekával se vyraz nebo volani funkce");
}

 // <expr> -> <fun_call> 
 // <expr> -> expression <expr2>
void syntax_expr(){
    fprintf(stderr, "expr\n");
    token = getToken(&attribute);fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));   
    if(token == LEN || token == RBR || token == LBR || token == ZERO || token == KW_NIL || (token >= STRING && token <= EXP)){
        
        if(token == LEN || token == LBR || token == INT){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "integer");
            DLL_InsertLast(&assignExpr, attributeTemp);
        }
        else if(token == STRING){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "string");
            DLL_InsertLast(&assignExpr, attributeTemp);
        }
        else if(token == DOUBLE || token == EXP){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "number");
            DLL_InsertLast(&assignExpr, attributeTemp);
        }
        exprOutcome = KW_NIL;
        bottom_up();
        if(!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "integer")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "number")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "string")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        } else if (!strCmpConstStr(&varTableSearch(&varTable, currentVar)->type, "nil")){
            fprintf(stdout, "POPS LF@%s\n", strGetStr(&currentVar));
        }
        exprEnd = false;
        stack_delete(s);
        syntax_expr2();
    }
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        //token = getToken(&attribute);fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        strCopyString(&currentVar, &attribute);
        if(funcTableSearch(&funcTable, currentVar)){
            
            if(!DLL_Compare(&currentList, &funcTableSearch(&funcTable, currentVar)->returnParam)) 
                errorMessage(ERR_ASSIGN, "Funkce vraci hodnoty ktere se neshoduji s identifikatory");

            DLL_Dispose(&currentList);
            DLL_Init(&currentList);

            strCopyString(&funcName, &attribute);

            token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
            syntax_fun_call();
        }
        else if(varTableSearch(&varTable, currentVar)){
            DLL_InsertLast(&assignExpr, varTableSearch(&varTable, currentVar)->type);
            exprOutcome = KW_NIL;
            bottom_up();
            exprEnd = false;
            stack_delete(s);
            syntax_expr2();
        }  
        else errorMessage(ERR_NONDEF, "Nedefinovany vyraz nebo volani funkce");  
    }
    else errorMessage(ERR_SYNTAX, "Očekával se vyraz nebo volani funkce");  
}

// <expr2> -> COMMA expression <expr2>
// <expr2> -> epsilon
void syntax_expr2(){
    fprintf(stderr, "expr2\n");    
    if(token == COMMA){
        token = getToken(&attribute);fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
        if (!((token >= STRING && token <= NEQUAL) || token == LEN)) errorMessage(ERR_SYNTAX, "Očekával se vyraz");  
        fprintf(stdout, "PUSHS int@%s\n", strGetStr(&attribute));
        if(token == LEN || token == LBR || token == INT){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "integer");
            DLL_InsertLast(&assignExpr, attributeTemp);
        }
        else if(token == STRING){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "string");
            DLL_InsertLast(&assignExpr, attributeTemp);
        }
        else if(token == DOUBLE || token == EXP){
            strClear(&attributeTemp);
            strAddString(&attributeTemp, "number");
            DLL_InsertLast(&assignExpr, attributeTemp);
        }
        else if(token == ID){
            if(varTableSearch(&varTable, attribute) == NULL) errorMessage(ERR_NONDEF, "Nedefinované proměná ve vyrazu");
            DLL_InsertLast(&assignExpr, varTableSearch(&varTable, currentVar)->type);            
        }  
        exprOutcome = KW_NIL;
        bottom_up();
        exprEnd = false;
        stack_delete(s);
        syntax_expr2();
    }
}

// <type> -> KW_STR
// <type> -> KW_INT
// <type> -> KW_NUM
bool syntax_type(){
    fprintf(stderr, "type\n");
    if(token == KW_STR || token == KW_INT || token == KW_NUM || token == KW_NIL) return true;
    else return false;
}

// <ID_next> -> COMMA ID <ID_next>
// <ID_next> -> epsilon
void syntax_ID_next(){
    fprintf(stderr, "ID_next\n");

    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
    
    if(!varTableSearch(&varTable, attribute)) errorMessage(ERR_NONDEF, "Nedefinovana promenna v assign");
    DLL_InsertLast(&currentList, varTableSearch(&varTable, attribute)->type);

    token = getToken(&attribute); fprintf(stderr, "%-15s |%s\n", printState(token), strGetStr(&attribute));
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
    return (s->top == -1);
}

// Uložení tokenu na zásobník
void stack_push(Stack *s, int token){
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

int stack_highest(Stack *s){
    int result = s->top;
    while(result >= 0){
        if(s->arr[result] != 100) return result;

        result--; 
    }
    return result;
}

void stack_handle(Stack *s){
    int result = s->top;
    while(result >= 0){
        if(s->arr[result] != E) break;
        result --;
    }
    if(s->top - result >= 2) errorMessage(ERR_TYPE_CMP, "Moc E vedle sebe");

    for(int i = 0; i < s->top - result; i++){
        s->arr[s->top - i + 1] = s->arr[s->top - i];
    }
    s->arr[result + 1] = HANDLE;
    s->top++;
}

void stack_rules(Stack *s){
    int result = s->top;
    while(result >= 0){
        if(s->arr[result] == HANDLE) break;
        result --;
    }
    if(result == -1) errorMessage(ERR_TYPE_CMP, "Ve vyrazu je chyba");

    if(s->top - result == 1){
        if(s->arr[s->top] == ID || s->arr[s->top] == ZERO || s->arr[s->top] == KW_NIL || (s->arr[s->top] >= STRING && s->arr[s->top] <= EXP)){
            s->top--; 
            s->arr[s->top] = E;
        }

        else errorMessage(ERR_TYPE_CMP, "Pravidlo nejde");
    }
    else if(s->top - result == 2){
        if(s->arr[s->top] == E && s->arr[s->top - 1] == LEN){
            s->top -= 2;
            s->arr[s->top] = E;
        }

        else errorMessage(ERR_TYPE_CMP, "Pravidlo nemuze");
    
    }

    else if(s->top - result == 3){
        if( (s->arr[s->top] == E) &&
            (s->arr[s->top - 1] == CONCAT || (s->arr[s->top - 1] >= ADD && s->arr[s->top - 1] <= NEQUAL)) &&
            (s->arr[s->top - 2]== E) )
        {
            s->top -= 3;
            s->arr[s->top] = E;
        }

        else errorMessage(ERR_TYPE_CMP, "Pravidlo nelze");
    }

    else errorMessage(ERR_TYPE_CMP, "Nesedi zadne pravidlo");
    
}