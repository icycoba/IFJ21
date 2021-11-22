/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      Převzato ze stránek IFJ projektu. (http://www.fit.vutbr.cz/study/courses/IFJ/public/project/)
 * 
 * Soubor:
 *      @file ilist.h
 * 
 * Popis:
 *      @brief Seznam pro generování instrukcí.
 */
#pragma once

#include <stdio.h>
#include <malloc.h>

// Vysvětlivky sekcí instrukcí na odkazu:
// https://wis.fit.vutbr.cz/FIT/st/cfs.php.cs?file=%2Fcourse%2FIFJ-IT%2Fprojects%2Fifj2021.pdf#17
enum instruction{
  // 10.4.1
  I_MOVE,
  I_CREATEFRAME,
  I_PUSHFRAME,
  I_POPFRAME,
  I_DEFVAR,
  I_CALL,
  I_RETURN,
  // 10.4.2
  I_PUSHS,
  I_POPS,
  I_CLEARS,
  // 10.4.3
  I_ADD,
  I_SUB,
  I_MUL,
  I_DIV,
  I_IDIV,
  I_ADDS,
  I_SUBS,
  I_MULS,
  I_DIVS,
  I_IDIVS,
  I_LT,
  I_GT,
  I_EQ,
  I_LTS,
  I_GTS,
  I_EQS,
  I_AND,
  I_OR,
  I_NOT,
  I_ANDS,
  I_ORS,
  I_NOTS,
  I_INT2FLOAT,
  I_FLOAT2INT,
  I_INT2CHAR,
  I_STRI2INT,
  I_INT2FLOATS,
  I_FLOAT2INTS,
  I_INT2CHARS,
  I_STRI2INTS,
  // 10.4.4
  I_READ,
  I_WRITE,
  // 10.4.5
  I_CONCAT,
  I_STRLEN,
  I_GETCHAR,
  I_SETCHAR,
  // 10.4.6
  I_TYPE,
  // 10.4.7
  I_LABEL,
  I_JUMP,
  I_JUMPIFEQ,
  I_JUMPIFNEQ,
  I_JUMPIFEQS,
  I_JUMPIFNEQS,
  I_EXIT,
  // 10.4.8
  I_BREAK,
  I_DPRINT,
  // Unknown instrukce
  I_UNKNOWN
};

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;
    
typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);
