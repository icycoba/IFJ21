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

#define SYNTAX_OK 0

int token;
string attribute;
symTableNodePtr funcTable;
symTableNodePtr varTable;

int parser();

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
void syntax_ID_assingn_or_fun();
void syntax_var_init();
void syntax_expr();
void syntax_expr2();
void syntax_ID_next();

bool syntax_type();