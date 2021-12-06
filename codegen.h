/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Martin Hlinský (xhlins01)
 * 
 * Soubor:
 *      @file codegen.h
 * 
 * Popis:
 *      @brief Generování tříadresného kódu
 */

#pragma once

#include "symtable.h"

// Každou instrukci následuje znak \n
void writeInstruction();

void generateProlog();
void generatePredefined();

void generateLabel(string *attribute);
void generateLabelEnd();
void generateCall(string *attribute);
void generateMain();
void generateVar(string *attribute);
void generateVar2(int type);
void generateVar3();
void generatePushS(sType type, string *attribute);
void generatePopS(string *atrribute);

void generateWhile();
void generateIf();

void generateAddS();
void generateSubS();
void generateMulS();
void generateDivS();
void generateIDivS();