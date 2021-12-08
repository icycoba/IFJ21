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

    printf("\nDEFVAR GF@$__temp__\n");
    printf("DEFVAR GF@$__length__\n");
    printf("DEFVAR GF@$__counter__\n");

    printf("\nLABEL $readi\n");
    printf("PUSHFRAME\n");
    printf("READ LF@%%retval%%1 int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $readn\n");
    printf("PUSHFRAME\n");
    printf("READ LF@%%retval%%1 float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $reads\n");
    printf("PUSHFRAME\n");
    printf("READ LF@%%retval%%1 string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $tointeger\n");
    printf("PUSHFRAME\n");
    printf("FLOAT2INT LF@%%retval%%1 LF@%%inval%%1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $substr\n");
    printf("PUSHFRAME\n");
    printf("STRLEN GF@$__length__ LF@%%inval%%1\n");
    printf("ADD GF@$__length__ GF@$__length__ int@1\n");
    printf("SUB GF@$__length__ GF@$__length__ LF@%%inval%%3\n");
    printf("MOVE GF@$__counter__ LF@%%inval%%2\n");
    printf("LABEL $substr$while$1\n");
    printf("JUMPIFEQ $substr$while$1$end GF@$__counter__ GF@$__length__\n");
    printf("GETCHAR GF@$__temp__ LF@%%inval%%1 GF@$__counter__\n");
    printf("CONCAT LF@%%retval%%1 LF@%%retval%%1 GF@$__temp__\n");
    printf("ADD GF@$__counter__ GF@$__counter__ int@1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $ord\n");
    printf("PUSHFRAME\n");
    printf("STRI2INT LF@%%retval%%1 LF@%%inval%%1 LF@%%inval%%2\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $chr\n");
    printf("PUSHFRAME\n");
    printf("INT2CHAR LF@%%retval%%1 LF@%%inval%%1\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");

}