
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
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
    /* Inicializace všech položek seznamu */
    list->firstElement  = NULL;
    list->activeElement = NULL;
    list->lastElement   = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    /* Aktualní a nasledující položky seznamu */
    DLLElementPtr activeElement = list->firstElement;
    DLLElementPtr nextElement;

    /* Pokud prvek existuje, zrušíme ho a nastavíme aktuální na nasledující */
    while (activeElement != NULL) {
        nextElement = activeElement->nextElement;
        free(activeElement);
        activeElement = nextElement;
    }

    DLL_Init(list);
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    /* Alokace pamětí pro nový prvek */
    DLLElementPtr newElement = malloc(sizeof(struct DLLElement));

    /* Pokud nepodařilo, vypis upozornění a konec */
    if (!newElement) {
        DLL_Error();
        return;
    }

    /* Vložení a inicializace prvku */
    newElement->nextElement = list->firstElement;
    newElement->previousElement = NULL;
    newElement->data = data;

    /* Změníme první prvek seznamu na vložený */
    list->firstElement = newElement;

    /**
     * Pokud seznam obsahuje pouze jeden prvek, označíme ho taky jako poslední.
     * Jinak spojíme nasledujicí prvek s prvním.
     */
    if (!newElement->nextElement) {
        list->lastElement = newElement;
    } else {
        newElement->nextElement->previousElement = newElement;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    /* Alokace pamětí pro nový prvek */
    DLLElementPtr newElement = malloc(sizeof(struct DLLElement));

    /* Pokud nepodařilo, vypis upozornění a konec */
    if (!newElement) {
        DLL_Error();
        return;
    }

    /* Vložení a inicializace prvku */
    newElement->nextElement = NULL;
    newElement->previousElement = list->lastElement;
    newElement->data = data;

    /* Změníme poslední prvek seznamu na vložený */
    list->lastElement = newElement;

    /**
     * Pokud seznam obsahuje pouze jeden prvek, označíme ho taky jako první.
     * Jinak spojíme předposlední prvek s posledním.
     */
    if (!newElement->previousElement) {
        list->firstElement = newElement;
    } else {
        newElement->previousElement->nextElement = newElement;
    }
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
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    /* Kontrola na prázdný seznam */
    if (!list->firstElement) {
        DLL_Error();
        return;
    }

    /* Ulkládáme do parametru hodnotu prvního prvku */
    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    /* Kontrola na prázdný seznam */
    if (!list->firstElement) {
        DLL_Error();
        return;
    }

    /* Ukládáme do parametru hodnotu posledního prvku */
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
    /* Kontrola na prázdný seznam */
    if (!list->firstElement) {
        return;
    }

    /* Uložíme ukazatel na první prvek */
    DLLElementPtr tmpElement = list->firstElement;

    /**
     * Pokud seznam obsahoval pouze jeden prvek, nastavíme
     * první a poslední prvek na hodnotu NULL.
     * Jinak změníme první prvek seznamu na nasledující
     * a nastavíme ho předcházející prvek na hodnotu NULL
     */
    if (!tmpElement->nextElement) {
        list->firstElement = NULL;
        list->lastElement = NULL;
    } else {
        list->firstElement = tmpElement->nextElement;
        tmpElement->nextElement->previousElement = NULL;
    }

    /* Pokud první prvek byl aktivní, aktivita se ztrácí */
    if (tmpElement == list->activeElement) {
        list->activeElement = NULL;
    }

    /* Rušíme první prvek */
    free(tmpElement);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    /* Kontrola na prázdný seznam */
    if (!list->firstElement) {
        return;
    }

    /* Uložíme ukazatel na poslední prvek */
    DLLElementPtr tmpElement = list->lastElement;

    /**
     * Pokud seznam obsahoval pouze jeden prvek, nastavíme
     * první a poslední prvek na hodnotu NULL.
     * Jinak změníme poslední prvek seznamu na předcházející
     * a nastavíme ho nasledující prvek na hodnotu NULL
     */
    if (!tmpElement->previousElement) {
        list->firstElement = NULL;
        list->lastElement = NULL;
    } else {
        list->lastElement = tmpElement->previousElement;
        tmpElement->previousElement->nextElement = NULL;
    }

    /* Pokud poslední prvek byl aktivní, aktivita se ztrácí */
    if (tmpElement == list->activeElement) {
        list->activeElement = NULL;
    }

    /* Rušíme poslední prvek */
    free(tmpElement);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    /* Kontrola dvou nutných podmínek */
    if (!DLL_IsActive(list) || list->activeElement == list->lastElement) {
        return;
    }

    /* Uložíme ukazatel na nasledující prvek za aktivím */
    DLLElementPtr tmpElement = list->activeElement->nextElement;

    /**
     * Pokud prvek, který potřebujeme smazat, je poslední,
     * nastavíme nasledující prvek u aktivního na hodnotu NULL
     * a jako poslední prvek zapíšeme aktivní.
     * Jinak nastavíme nasledujíci prvek u aktivního na
     * nasledující prvek toho, který potřebujeme smazat, a naopak
     */
    if (!tmpElement->nextElement) {
        list->activeElement->nextElement = NULL;
        list->lastElement = list->activeElement;
    } else {
        list->activeElement->nextElement = tmpElement->nextElement;
        tmpElement->nextElement->previousElement = list->activeElement;
    }

    /* Rušíme prvek seznamu za aktivním prvkem */
    free(tmpElement);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    /* Kontrola dvou nutných podmínek */
    if (!DLL_IsActive(list) || list->activeElement == list->firstElement) {
        return;
    }

    /* Uložíme ukazatel na prvek před aktivním */
    DLLElementPtr tmpElement = list->activeElement->previousElement;

    /**
     * Pokud prvek, který potřebujeme smazat, je první,
     * nastavíme prvek před aktivím na hodnotu NULL
     * a jako první prvek zapíšeme aktivní.
     * Jinak nastavíme předchazející prvek u aktivního na
     * předchazející prvek toho, který potřebujeme smazat, a naopak
     */
    if (!tmpElement->previousElement) {
        list->activeElement->previousElement = NULL;
        list->firstElement = list->activeElement;
    } else {
        list->activeElement->previousElement = tmpElement->previousElement;
        tmpElement->previousElement->nextElement = list->activeElement;
    }

    /* Rušíme prvek seznamu před aktivním prvkem */
    free(tmpElement);
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
void DLL_InsertAfter( DLList *list, int data ) {
    if (!DLL_IsActive(list)) {
        return;
    }

    DLLElementPtr newElement = malloc(sizeof(struct DLLElement));
    if (!newElement) {
        DLL_Error();
        return;
    }

    DLLElementPtr tmpElement = list->activeElement->nextElement;

    if (!tmpElement) {
        newElement->nextElement = NULL;
        list->lastElement = newElement;
    } else {
        tmpElement->previousElement = newElement;
        newElement->nextElement = tmpElement;
    }

    newElement->previousElement = list->activeElement;
    list->activeElement->nextElement = newElement;
    newElement->data = data;
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
void DLL_InsertBefore( DLList *list, int data ) {
    if (!DLL_IsActive(list)) {
        return;
    }

    DLLElementPtr newElement = malloc(sizeof(struct DLLElement));
    if (!newElement) {
        DLL_Error();
        return;
    }

    DLLElementPtr tmpElement = list->activeElement->previousElement;

    if (!tmpElement) {
        newElement->previousElement = NULL;
        list->firstElement = newElement;
    } else {
        tmpElement->nextElement = newElement;
        newElement->previousElement = tmpElement;
    }

    newElement->nextElement = list->activeElement;
    list->activeElement->previousElement = newElement;
    newElement->data = data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    if (!DLL_IsActive(list)) {
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
void DLL_SetValue( DLList *list, int data ) {
    if (!DLL_IsActive(list)) {
        return;
    }

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
    if (!DLL_IsActive(list)) {
        return;
    }

    DLLElementPtr tmpElement = list->activeElement->nextElement;
    list->activeElement = (tmpElement) ? tmpElement : NULL;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if (!DLL_IsActive(list)) {
        return;
    }

    DLLElementPtr tmpElement = list->activeElement->previousElement;
    list->activeElement = (tmpElement) ? tmpElement : NULL;
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
    return (list->activeElement) ? 1 : 0;
}

/* Konec c206.c */
