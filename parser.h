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
 *      @file parser.h
 * 
 * Popis:
 *      @brief Modul pro provedení syntaktické a sémantické analýzy
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"
#include "codegen.h"

#define SYNTAX_OK 0
#define STACK_MAX_SIZE 30

#define E 100
#define HANDLE 101

int token;
int exprOutcome;
bool exprEnd;
bool skip;
//bool mainLabel;
string attribute;
string attributeTemp;
string currentFunc;
string currentVar;
string funcName;

DLList currentList;
DLList assignExpr;

funcTableNodePtr funcTable;
varTableNodePtr varTable;

typedef struct{
    int arr[STACK_MAX_SIZE];
    int top;
} Stack;

Stack *s;

int parser();

void bottom_up();
void syntax_program();
void syntax_prolog();
void syntax_fun_dec_def_call();
void syntax_fun_call();
void syntax_param_type();
void syntax_param_type2();
void syntax_type_rtrn();
void syntax_type_rtrn2();
void syntax_fun_params();
void syntax_fun_params2();
void syntax_fun_call_params();
void syntax_fun_call_params2();
void syntax_stmts();
void syntax_stmt();
void syntax_else();
void syntax_var_init();
void syntax_init();
void syntax_expr();
void syntax_expr2();
void syntax_ID_next();

bool syntax_type();

void stack_init(Stack *);
int stack_isFull(const Stack *);
int stack_isEmpty(const Stack *);
void stack_push(Stack *, int);
void stack_pop(Stack *);
void stack_top(const Stack *, int *);
void stack_delete(Stack *);
int stack_highest(Stack *);
void stack_handle(Stack *);
void stack_rules(Stack *);