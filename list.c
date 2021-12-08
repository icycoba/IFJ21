/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek (xrysan05)
 * 
 * Soubor:
 *      @file list.c
 * 
 * Popis:
 *      @brief Dvojsměrný seznam
 */

#include "list.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    DLLElementPtr elemPtr; 
    while(list->firstElement != NULL)
    {
        if(list->firstElement == list->lastElement)
        {
            elemPtr = list->firstElement;
            free(elemPtr);
            list->firstElement = NULL;
            list->lastElement = NULL;
            list->activeElement = NULL;
            break;    
        }
        elemPtr = list->firstElement;
        list->firstElement = list->firstElement->nextElement;
        list->firstElement->previousElement = NULL;
        free(elemPtr);
    }
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, string data ) {
    DLLElementPtr elemPtr = malloc(sizeof(struct DLLElement));

    if (!elemPtr){
        DLL_Error();
        return;
    }
    elemPtr->data = data;
    elemPtr->nextElement = list->firstElement;
    elemPtr->previousElement = NULL;

    if(list->firstElement != NULL)
        list->firstElement->previousElement = elemPtr;
    else
        list->lastElement = elemPtr;

    list->firstElement = elemPtr;    
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, string data ) {
    DLLElementPtr elemPtr = malloc(sizeof(struct DLLElement));

    if (!elemPtr){
        DLL_Error();
        return;
    }

    string new;
    if(strInit(&new)) errorMessage(ERR_INTERNAL, "Chyba alokace řetězce");
    strCopyString(&new, &data);

    elemPtr->data = new;
    elemPtr->previousElement = list->lastElement;
    elemPtr->nextElement = NULL;

    if(list->lastElement != NULL)
        list->lastElement->nextElement = elemPtr;
    else
        list->firstElement = elemPtr;

    list->lastElement = elemPtr;   
}

int DLL_Compare(DLList *list1, DLList *list2){
    
    DLLElementPtr first = malloc(sizeof(struct DLLElement));
    DLLElementPtr second = malloc(sizeof(struct DLLElement));
    
    first = list1->firstElement;
    second = list2->firstElement;
    while (first != NULL && second != NULL){
        fprintf(stderr, "[%s, %s]", strGetStr(&first->data), strGetStr(&second->data));
        if(strCmpString(&first->data, &second->data) && 
           (strCmpConstStr(&first->data,"integer") || strCmpConstStr((&second->data), "number"))) break;     

        first = first->nextElement;
        second = second->nextElement;
    }
    if (first == NULL && second == NULL) return 1;
    if (first == NULL && second != NULL) return -1;
    return 0; 
}

int DLL_length(DLList *list){
    int count = 0;
    DLL_First(list);
    while (DLL_IsActive(list))
    {
        count++;
        DLL_Next(list);
    }
    return count;
    
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, string *dataPtr ) {
    if(list->firstElement == NULL){
        DLL_Error();
        return;
    }
    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, string *dataPtr ) {
    if(list->lastElement == NULL){
        DLL_Error();
        return;
    }
    *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    DLLElementPtr elemPtr;
    if (list->firstElement != NULL)
    {
        elemPtr = list->firstElement;
        if (list->activeElement == list->firstElement)
            list->activeElement = NULL;
        if (list->firstElement == list->lastElement)
        {
            list->firstElement = NULL;
            list->lastElement = NULL;            
        }
        else
        {
            list->firstElement = list->firstElement->nextElement;
            list->firstElement->previousElement = NULL;
        }

        free(elemPtr);        
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    DLLElementPtr elemPtr;
    if (list->lastElement != NULL)
    {
        elemPtr = list->lastElement;
        if (list->activeElement == list->lastElement)
            list->activeElement = NULL;
        if (list->firstElement == list->lastElement)
        {
            list->firstElement = NULL;
            list->lastElement = NULL;            
        }
        else
        {
            list->lastElement = list->lastElement->previousElement;
            list->lastElement->nextElement = NULL;
        }

        free(elemPtr);        
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {    
    if (list->activeElement != NULL)
    {
        if (list->activeElement->nextElement != NULL)
        {
            DLLElementPtr elemPtr;
            elemPtr = list->activeElement->nextElement;
            list->activeElement->nextElement = elemPtr->nextElement;

            if (elemPtr == list->lastElement)
                list->lastElement = list->activeElement;                  
            else
                elemPtr->nextElement->previousElement = list->activeElement;                     

            free(elemPtr);  
        }      
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if (list->activeElement != NULL)
    {
        if (list->activeElement->previousElement != NULL)
        {
            DLLElementPtr elemPtr;
            elemPtr = list->activeElement->previousElement;
            list->activeElement->previousElement = elemPtr->previousElement;

            if (elemPtr == list->firstElement)
                list->firstElement = list->activeElement;                  
            else
                elemPtr->previousElement->nextElement = list->activeElement;                     

            free(elemPtr);  
        }      
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, string data ) {
    DLLElementPtr elemPtr = malloc(sizeof(struct DLLElement));

    if (!elemPtr){
        DLL_Error();
        return;
    }

    elemPtr->data = data;
    elemPtr->nextElement = list->activeElement->nextElement;
    elemPtr->previousElement = list->activeElement;
    list->activeElement->nextElement = elemPtr;

    if(list->activeElement == list->lastElement)
        list->lastElement = elemPtr;
    else
        elemPtr->nextElement->previousElement = elemPtr;  

     
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, string data ) {
    DLLElementPtr elemPtr = malloc(sizeof(struct DLLElement));

    if (!elemPtr){
        DLL_Error();
        return;
    }

    elemPtr->data = data;
    elemPtr->previousElement= list->activeElement->previousElement;
    elemPtr->nextElement = list->activeElement;
    list->activeElement->previousElement = elemPtr;

    if(list->activeElement == list->firstElement)
        list->firstElement = elemPtr;
    else
        elemPtr->previousElement->nextElement = elemPtr;  
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, string *dataPtr ) {
    if (!DLL_IsActive(list)){
        DLL_Error();
        return;
    }

    *dataPtr = list->activeElement->data;

}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, string data ) {
    if (DLL_IsActive(list))
        list->activeElement->data = data;

}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    if (DLL_IsActive(list))
        list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if (DLL_IsActive(list))
        list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return (list->activeElement != NULL);
}

/* Konec c206.c */
