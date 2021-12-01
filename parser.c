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
    funcTableInit(&funcTable);
    if(strInit(&attribute)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    if(strInit(&attributeTemp)) errorMessage(ERR_INTERNAL, "Chyba alokace dočasného řetězce");

    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    syntax_program();

    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != EOFILE) errorMessage(ERR_SYNTAX, "Chybí EOF");

    strFree(&attribute);
    funcTableDispose(&funcTable);
    ////printf("syntakticka analyza probehla bez problemu\n");
    return SYNTAX_OK;  
}

void bottom_up(){
    ////printf("bottom-up\n");
    Stack *s;
    s = (Stack *) malloc(sizeof(Stack));
    stack_init(s);
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
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

            //printf("STACK_TOP: %d", s->arr[s->top]);
        }*/
        
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
    //if(!stack_isEmpty(s)) stack_delete(s);
    ////printf("bottom-up-end\n");
}

//// každá syntax funkce je jeden neterminál v gramatice ////

// <program> -> <prolog> <fun_dec_def_call> EOF
void syntax_program(){
    ////printf("program\n");
    if (token != EOFILE){
        syntax_prolog();
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_fun_dec_def_call();        
    }

    //TODO if token != EOFILE => SYNTAX / SEMANTICS ERROR
}

// <prolog> -> KW_REQUIRE "ifj21"
void syntax_prolog(){
    //printf("prolog\n");
    ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != KW_REQUIRE) errorMessage(ERR_SYNTAX, "V prologu chybí klíčové slovo require");

    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if (token != STRING || strCmpConstStr(&attribute, "\"ifj21\"")) errorMessage(ERR_SYNTAX, "Klíčové slovo require musí následovat řetězec \"ifj21\"");
    generateProlog();
}

// <fun_dec_def_call> -> KW_GLOBAL  ID DOUBLEDOT    KW_FUNC         LBR <param_type>    RBR     <type_rtrn> <fun_dec_def_call>
// <fun_dec_def_call> -> KW_FUNC    ID LBR          <fun_params>    RBR <type_rtrn>     <stmts> KW_END      <fun_dec_def_call>
// <fun_dec_def_call> -> <fun_call> <fun_dec_def_call>
// <fun_dec_def_call> -> epsilon
void syntax_fun_dec_def_call(){
    //printf("fun_dec_def_call\n");
    if (token == KW_GLOBAL){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX /*MOZNA ERR_SYNTAX TODO nejsem si jisty jaky tady bude error -xhlins01*/, "Pokus o definici non-ID");
        //je atribut jiz declared?
        if(funcTableSearch(&funcTable, attribute)){
            errorMessage(ERR_NONDEF, "Pokus o redeklaraci funkce");
        } else{
            funcTableInsert(&funcTable, attribute, NULL);
        }

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak : v deklaraci funkce");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != KW_FUNC) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo function v deklaraci funkce");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak ( v deklaraci funkce");
        syntax_param_type();

        //token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak ) v deklaraci funkce");
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak : v deklaraci funkce");
        syntax_type_rtrn();

        syntax_fun_dec_def_call();
    }
    else if (token == KW_FUNC){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if (token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");
        generateLabel(&attribute);

        // TODO sémantika
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != LBR) errorMessage(ERR_SYNTAX, "Očekával se znak (");

        syntax_fun_params();
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se znak )");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT) syntax_type_rtrn();
        //syntax_type_rtrn();
        syntax_stmts();

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se klíčové slovo end na konci funkce");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));

        syntax_fun_dec_def_call();
    }
    else if(token == ID){
        generateMain();
        generateCall(&attribute);
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
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
    //printf("fun_call\n");
    
    if (token == LBR){
        syntax_fun_call_params();
        
        //token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != RBR) errorMessage(ERR_SYNTAX, "Očekával se se znak ')'");
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        //printf("yeah\n");
    }
    else errorMessage(ERR_SYNTAX, "Očekával se znak '('");
}

// <param_type> -> <type> <param_type2>
// <param_type> -> epsilon
void syntax_param_type(){
    //printf("param_type\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(syntax_type()){
        syntax_param_type2();
    }
    //else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <param_type2> -> COMMA <type> <param_type2>
// <param_type2> -> epsilon
void syntax_param_type2(){
    //printf("param_type2\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_param_type2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
    //else errorMessage(ERR_SYNTAX, "Očekával se znak  ','");
}

// <type_rtrn> -> DOUBLEDOT <type> <type_rtrn2>
// <type_rtrn> -> epsilon
void syntax_type_rtrn(){
    //printf("type_rtrn\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(syntax_type()) syntax_type_rtrn2();
    else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
}

// <type_rtrn2> -> COMMA <type> <type_rtrn2>
// <type_rtrn2> -> epsilon
void syntax_type_rtrn2(){
    //printf("type_rtrn2\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(syntax_type()) syntax_type_rtrn2();
        else errorMessage(ERR_SYNTAX, "Očekával se typ proměnné");
    }
}

// <fun_params> -> ID DOUBLEDOT <type> <fun_params2>
// <fun_params> -> epsilon
void syntax_fun_params(){
    //printf("fun_params\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == DOUBLEDOT){
            token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(syntax_type()) syntax_fun_params2();
            else errorMessage(ERR_SYNTAX, "Očekával se typ funkce");
        }
        else errorMessage(ERR_SYNTAX, "Očekával se znak ':'");
    }
}

// <fun_params2> -> COMMA ID DOUBLEDOT <type> <fun_params2>
// <fun_params2> -> epsilon
void syntax_fun_params2(){
    //printf("fun_params2\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == ID){
            token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
            if(token == DOUBLEDOT){
                token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
                if(syntax_type()) syntax_fun_params2();
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
    //printf("fun_call_params\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ID || (token >= STRING && token <= EXP)) {
        syntax_fun_call_params2();
        //token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }
}

// <fun_call_params2> -> COMMA ID <fun_call_params2>
// <fun_call_params2> -> epsilon
void syntax_fun_call_params2(){
    //printf("fun_call_params2\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == ID || (token >= STRING && token <= EXP)) syntax_fun_call_params2();           
        
        else errorMessage(ERR_SYNTAX, "Očekávalo se ID");
        
    }
    
}

// <stmts> -> <stmt> <stmts>
// <stmts> -> epsilon
void syntax_stmts(){
    //printf("stmts\n");
    //token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
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
    //printf("stmt\n");
    if(token == KW_LOCAL){
        //printf("stmt-local\n");
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != ID) errorMessage(ERR_SYNTAX, "Očekával se token ID");
        generateVar(&attribute);

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token != DOUBLEDOT) errorMessage(ERR_SYNTAX, "Očekával se znak :");

        token = getToken(&attribute); printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(!syntax_type()) errorMessage(ERR_SYNTAX, "Očekával se typ");;

        syntax_var_init();
    }     
    else if(token == KW_IF){
        //printf("stmt-if\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        bottom_up();
        if(token != KW_THEN) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"then\"");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        //printf("befelemep4684165165165řščěš\n");
        syntax_stmts();
        
        //printf("befelemepeseveze265464\n");
        if(token == KW_ELSE) syntax_else();
        

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        if(token == KW_END) //printf("Je tu slovo \"end\"\n");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    }     
    else if(token == KW_WHILE){
        //printf("stmt-while\n");
        //TODO - volani bottom-up analyzy která určí jestli je tu validní terminál a vyhodnotí ho
        bottom_up();
        
        if(token != KW_DO) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"do\"");

        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_stmts();

        
        if(token != KW_END) errorMessage(ERR_SYNTAX, "Očekávalo se slovo \"end\"");
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    } 
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        //printf("stmt-id-or-fun\n");
        strCopyString(&attributeTemp, &attribute);
        
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if(token == LBR){
            generateCall(&attributeTemp);
            syntax_fun_call();
        }
        else if(token == COMMA){
            syntax_ID_next();        
            if(token != ASSIGN) errorMessage(ERR_SYNTAX, "Očekával se znak =");
            syntax_expr();
        }
        else if(token == ASSIGN){
                     
            syntax_expr();
        }
        else errorMessage(ERR_SYNTAX, "Očekával se znak , nebo ( nebo =");
    } 
    else if(token == KW_RETURN){
        //printf("stmt-return\n");
        token = getToken(&attribute);//printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
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
    //printf("else\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != KW_END) {
        //printf("befelemepeseveze2\n");
        syntax_stmts();  //printf("befelemepeseveze1\n");
    } 
    //printf("befelemepeseveze\n");

}


// <var_init> -> ASSIGN <init>
// <var_init> -> epsilon
void syntax_var_init(){
    //printf("var_init\n");
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == ASSIGN){
        token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        syntax_init();
    }
}

// <init> -> expression
// <init> -> <fun_call>
void syntax_init(){
    //printf("init\n");
    if(token == LEN || token == LBR || token == ZERO || token == KW_NIL || (token >= STRING && token <= EXP)){
        bottom_up();
    }
    else if(token == ID || (token <= F_CHR && token >= F_READS)){
        token = getToken(&attribute);//printf("%-15s |%s\n", printState(token), strGetStr(&attribute));

        if(token == LBR) syntax_fun_call();

        else if((token >= ADD && token <= DIV_WHOLE) || token == CONCAT) bottom_up();
    }
    else errorMessage(ERR_SYNTAX, "Očekával se vyraz nebo volani funkce");
}

 // <expr> -> <fun_call> 
 // <expr> -> expression <expr2>
void syntax_expr(){
    //printf("expr\n");
    token = getToken(&attribute);//printf("%-15s |%s\n", printState(token), strGetStr(&attribute));   
    if(token == LEN || token == RBR || token == LBR || token == ZERO || token == KW_NIL || (token >= STRING && token <= EXP)){
        bottom_up();
        syntax_expr2();
    }
    else if(token == ID){
        token = getToken(&attribute);//printf("%-15s |%s\n", printState(token), strGetStr(&attribute));

        if(token == LBR){
            syntax_fun_call();
        }
        else if(token >= ADD && token <= DIV_WHOLE){ 
            bottom_up();
            syntax_expr2();
        }
        else errorMessage(ERR_SYNTAX, "Očekával se vyraz nebo volani funkce");    
    }
}

// <expr2> -> COMMA expression <expr2>
// <expr2> -> epsilon
void syntax_expr2(){
    //printf("expr2\n");    
    if(token == COMMA){
        token = getToken(&attribute);//printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
        if ((token >= STRING && token <= NEQUAL) || token == LEN) errorMessage(ERR_SYNTAX, "Očekával se vyraz");  
        bottom_up();
        syntax_expr2();
    }
}

// <type> -> KW_STR
// <type> -> KW_INT
// <type> -> KW_NUM
bool syntax_type(){
    //printf("type\n");
    if(token == KW_STR || token == KW_INT || token == KW_NUM || token == KW_NIL) return true;
    else return false;
}

// <ID_next> -> COMMA ID <ID_next>
// <ID_next> -> epsilon
void syntax_ID_next(){
    //printf("ID_next\n");

    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token != ID) errorMessage(ERR_SYNTAX, "Očekávalo se ID");
    
    token = getToken(&attribute); ////printf("%-15s |%s\n", printState(token), strGetStr(&attribute));
    if(token == COMMA) syntax_ID_next();
}

// Inicializace zásobníku
void stack_init(Stack *s){
    if(s==NULL){
        errorMessage(ERR_INTERNAL, "Chyba inicializace zásobníku");
    }
    else s->top = -1;
}

// Je zásobník plný?
int stack_isFull(const Stack *s){
    return (s->top == STACK_MAX_SIZE-1);
}

// Je záspbník prázdný?
int stack_isEmpty(const Stack *s){
    return (s->top == STACK_MAX_SIZE - 1);
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