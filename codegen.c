/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 * 
 * Soubor:
 *      @file codegen.c
 * 
 * Popis:
 *      @brief Generování tříadresného kódu
 */

#include "codegen.h"

/**
 * Zásady pojmenování
 * 
 * $a       - funkce
 * $%a%1    - jmeno promenne, parametr a cislo parametru funkce
 * $?t      - navratovy typ 
 * %a%1     - jmeno promenne
*/

/*
void storeInstruction(tInstr instruction, char* addr1, char* addr2, char* addr3){
    tInstr instruction;
    instruction.addr1 = addr1;
    instruction.addr2 = addr2;
    instruction.addr3 = addr3;
}
*/

// První se generuje prolog, pak hned skok na "vnitřní main" ($__main__)

void generateProlog(){
    printf(".IFJcode21\n");
    printf("JUMP $__main__\n");
}

void generatePredefined(){
    printf("\nLABEL $readi\n");
    printf("PUSHFRAME\n");
    //printf("DEFVAR LF@ret\n");
    printf("READ LF@%%retval%%1 int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $readn\n");
    printf("PUSHFRAME\n");
    //printf("DEFVAR LF@ret\n");
    printf("READ LF@%%retval%%1 float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $reads\n");
    printf("PUSHFRAME\n");
    //printf("DEFVAR LF@ret\n");
    printf("READ LF@%%retval%%1 string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //TODO vstupni params (promenlivy pocet)
    printf("LABEL $write\n");
    printf("PUSHFRAME\n");
    printf("WRITE LF@%%retval%%1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //TODO vstupni params
    printf("LABEL $tointeger\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@_\n");
    printf("DEFVAR LF@ret\n");
    printf("PUSHS float@\n");
    printf("PUSHS int@\n");
    printf("POPS LF@ret\n");
    printf("POPS LF@_\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $substr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@1\n");
    printf("DEFVAR LF@2n\n");
    printf("DEFVAR LF@ret\n");
    printf("GETCHAR LF@ret LF@1 LF@2\n");
    printf("PUSHS string@\n");
    printf("POPS LF@ret\n");
    printf("POPS LF@2\n");
    printf("POPS LF@1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $ord\n");

    printf("RETURN\n");

    printf("LABEL $chr\n");

    printf("RETURN\n\n");

}

void generateLabel(string *attribute){
    printf("LABEL $%s\n", strGetStr(attribute));
    /*Parametry dávat před call funkce :sunglasses: -xhlins01*/
    // POKUD BUDE MÍT PARAMETRY, TAK PRINT PUSHFRAME A NA KONCI POPFRAME
    /*char params[1][3];
    params[0][0] = "a";
    params[0][1] = "b";
    params[0][2] = "c";
    for(int i = 0; i < paramCount; i++){
        printf("DEFVAR LF@$%%%s%%%d\n", params[0][i]);
    }*/
}

void generateLabelEnd(){
    //printf("POPFRAME\n");
    printf("RETURN\n");
}

void generateCall(string *attribute){
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%%retval%%1\n");
    printf("DEFVAR TF@%%retval%%2\n");
    printf("DEFVAR TF@%%retval%%3\n");
    printf("CALL $%s\n", strGetStr(attribute));
}

void generateMain(){
    printf("LABEL $__main__\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
}

void generateVar(string *attribute){
    printf("DEFVAR LF@%s\n", strGetStr(attribute));
}

void generateVar2(int type){
    switch(type){
        case KW_NIL:
            printf("co ti jebe -codegen\n");
            break;
        case KW_INT:
            printf("PUSHS int@");
            break;
        case KW_NUM:
            printf("PUSHS float@");
            break;
        case KW_STR:
            printf("PUSHS string@");
            break;
        default:
            break;
    }
    //printf("POPS LF@%s\n", strGetStr(attribute));
}

void generatePushS(sType type, string *attribute){
    switch(type){
        case T_NIL:
            printf("co ti jebe -codegen\n");
            break;
        case T_INT:
            printf("PUSHS int@%s\n", strGetStr(attribute));
            break;
        case T_DOUBLE:
            printf("PUSHS float@%s\n", strGetStr(attribute));
            break;
        case T_STRING:
            printf("PUSHS string@%s\n", strGetStr(attribute));
            break;
        default:
            break;
    }
}

void generatePopS(string *atrribute){
    printf("POPS LF@%s\n", strGetStr(atrribute));
}