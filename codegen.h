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

void generatePredefined();
