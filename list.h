/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek (xrysan05)
 * 
 * Soubor:
 *      @file list.h
 * 
 * Popis:
 *      @brief Dvojsměrný seznam
 */

#pragma once

#include "str.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

/** Globální proměnná - příznak ošetření chyby. */
extern int error_flag;
/** Globální proměnná - indikuje, zda byla operace řešena. */
extern int solved;

/** Prvek dvousměrně vázaného seznamu. */
typedef struct DLLElement {
	/** Užitečná data. */
	string data;
	/** Ukazatel na předcházející prvek seznamu. */
	struct DLLElement *previousElement;
	/** Ukazatel na následující prvek seznamu. */
	struct DLLElement *nextElement;
} *DLLElementPtr;

/** Dvousměrně vázaný seznam. */
typedef struct {
	/** Ukazatel na první prvek seznamu. */
	DLLElementPtr firstElement;
	/** Ukazatel na aktuální prvek seznamu. */
	DLLElementPtr activeElement;
	/** Ukazatel na posledni prvek seznamu. */
	DLLElementPtr lastElement;
} DLList;

void DLL_Init( DLList * );

void DLL_Dispose( DLList * );

void DLL_InsertFirst( DLList *, string );

void DLL_InsertLast( DLList *, string );

int DLL_Compare(DLList *, DLList *);

void DLL_First( DLList * );

void DLL_Last( DLList * );

void DLL_GetFirst( DLList *, string * );

void DLL_GetLast( DLList *, string * );

void DLL_DeleteFirst( DLList * );

void DLL_DeleteLast( DLList * );

void DLL_DeleteAfter( DLList * );

void DLL_DeleteBefore( DLList * );

void DLL_InsertAfter( DLList *, string );

void DLL_InsertBefore( DLList *, string );

void DLL_GetValue( DLList *, string * );

void DLL_SetValue( DLList *, string );

void DLL_Next( DLList * );

void DLL_Previous( DLList * );

int DLL_IsActive( DLList * );

/* Konec hlavičkového souboru c206.h */