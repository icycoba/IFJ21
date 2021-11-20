/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek   (xrysan05)
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

int parser();

int syntax_program();
int syntax_prolog();
int syntax_fun_dec_def_call();
int syntax_fun_call();
int syntax_param_type();
int syntax_param_type2();
int syntax_type_rtrn();
int syntax_type_rtrn2();
int syntax_fun_params();
int syntax_fun_params2();
int syntax_stmts();
int syntax_stmt();
int syntax_var_init();
int syntax_expr();
int syntax_expr2();
int syntax_type();
int syntax_ID_next();