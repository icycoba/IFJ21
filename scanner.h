/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři: 
 *      @author Martin Hlinský  (xhlins01)
 * 
 * Soubor:
 *      @file scanner.h
 * 
 * Popis:
 *      @brief Modul pro provedení lexikální analýzy
 */
 
#pragma once

#include <stdio.h>
#include <ctype.h>

#include "str.h"
#include "error.h"

enum state{
    // Nekoncové stavy
    S_START,
    S_DOT,
    S_STRSTART,
    S_STR1,
    S_STR2,
    S_STR2A,
    S_STR2B,
    S_STR3,
    S_STR3A,
    S_STR3B,
    S_INT,
    S_DOUBLE1,
    S_DOUBLE,
    S_EXP1,
    S_EXP2,
    S_EXP,
    S_ID,
    S_SUB,
    S_DIV,
    S_GT,
    S_LT,
    S_ASSIGN,
    S_NEQ,
    S_COMM_LINE,
    S_BLOCK1,
    S_BLOCK,
    S_BLOCK_END1,
    S_ZERO,
    S_KW,

    //Koncové stavy
    COMMA,
    DOT,
    CONCAT,
    DOUBLEDOT,
    STRING,
    INT,
    DOUBLE,
    EXP,
    ID,
    ADD,
    SUB,
    MUL,
    DIV,
    DIV_WHOLE,
    GT,
    GTE,
    LT,
    LTE,
    ASSIGN,
    EQUAL,
    NEQUAL,
    LBR,
    RBR,
    LSBR,
    RSBR,
    LCBR,
    RCBR,
    LEN,
    COMM_LINE_END,
    BLOCK_END,
    ZERO,
    EOFILE,

    //Klíčová slova
    KW_DO,
    KW_ELSE,
    KW_END,
    KW_FUNC,
    KW_GLOBAL,
    KW_IF,
    KW_INT,
    KW_LOCAL,
    KW_NIL,
    KW_NUM,
    KW_REQUIRE,
    KW_RETURN,
    KW_STR,
    KW_THEN,
    KW_WHILE,

    //Vestavěné funkce
    F_READS,
    F_READI,
    F_READN,
    F_WRITE,
    F_TOINTEGER,
    F_SUBSTR,
    F_ORD,
    F_CHR,

    UNKNOWN
 };

 int getToken(string *attribute);
 
 const char *printState(int);