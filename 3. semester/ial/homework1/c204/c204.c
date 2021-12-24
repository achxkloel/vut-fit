
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/* Seznam priorit (od nejnižší do nejvyšší) */
enum PrecedenceList {
    ADD_GROUP,  /* '+' a '-' */
    MUL_GROUP   /* '*' a '/' */
};

/**
 * Pomocná funkce isOperator
 *
 * @param inputChar Vstupní znak
 * @return TRUE pokud vstupní znak je operator, jinak FALSE
 */
int isOperator ( char inputChar ) {
    switch (inputChar) {
        case '+':
        case '-':
        case '*':
        case '/':
            return TRUE;
        default:
            return FALSE;
    }
}

/**
 * Pomocná funkce isOperand
 *
 * @param inputChar Vstupní znak
 * @return TRUE pokud vstupní znak je operand, jinak FALSE
 */
int isOperand ( char inputChar ) {
    if ((inputChar >= 'a' && inputChar <= 'z') ||
        (inputChar >= 'A' && inputChar <= 'Z') ||
        (inputChar >= '0' && inputChar <= '9')) {
        return TRUE;
    }

    return FALSE;
}

/**
 * Pomocná funkce getPrecedence
 *
 * @param op Znak operátoru
 * @return Precedence value nebo -1, když znak není operátor
 */
int getPrecedence ( char op ) {
    switch (op) {
        case '+':
        case '-': return ADD_GROUP;
        case '*':
        case '/': return MUL_GROUP;
        default: return -1;
    }
}

/**
 * Pomocná funkce cmpPrecedence
 *
 * @param fisrtOp První operátor
 * @param secondOp Druhý operátor
 * @return
 * >0 pokud první operátor má větší prioritu,
 * =0 pokud operátory mají stejnou prioritu,
 * \<0 pokud první operátor má menší prioritu
 */
int cmpPrecedence ( char firstOp, char secondOp ) {
    return getPrecedence(firstOp) - getPrecedence(secondOp);
}

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char cTop;
    while (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &cTop);

        if (cTop == '(') {
            Stack_Pop(stack);
            break;
        }

        postfixExpression[(*postfixExpressionLength)++] = cTop;
        Stack_Pop(stack);
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    /* Vložíme operátor na vrchol zásobníku pouze v případě, že: */

    /* Zásobník je prázdný */
    if (Stack_IsEmpty(stack)) {
        Stack_Push(stack, c);
        return;
    }

    /* Na vrcholu zásobníku je levá závorka */
    char cTop;
    Stack_Top(stack, &cTop);
    if (cTop == '(') {
        Stack_Push(stack, c);
        return;
    }

    /* Na vrcholu zásobníku je operátor s nižší prioritou */
    if (cmpPrecedence(cTop, c) < 0) {
        Stack_Push(stack, c);
        return;
    } else {
        Stack_Pop(stack);
        postfixExpression[(*postfixExpressionLength)++] = cTop;
        doOperation(stack, c, postfixExpression, postfixExpressionLength);
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    /* Alokace vystupního řetězce */
    char *postfixExpression = calloc(MAX_LEN, 1);
    if (!postfixExpression) {
        return NULL;
    }

    /* Alokace zásobniku */
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        free(postfixExpression);
        return NULL;
    }

    /* Inicializace zásobniku */
    Stack_Init(stack);

    /* Délka výstupního řetězce */
    unsigned postfixExpressionLength = 0;

    /* Main cycle */
    for (int i = 0; infixExpression[i] != 0; i++) {
        /**
         * Pokud aktuální znak je operand,
         * přidáme ho na konec výstupního řetězce
         */
        if (isOperand(infixExpression[i])) {
            postfixExpression[postfixExpressionLength++] = infixExpression[i];
        }

        /**
         * Pokud aktuální znak je levá závorka,
         * vložíme ji na vrchol zásobníku
         */
        if (infixExpression[i] == '(') {
            Stack_Push(stack, infixExpression[i]);
        }

        /**
         * Pokud aktuální znak je operátor,
         * zavoláme funkci doOperation
         */
        if (isOperator(infixExpression[i])) {
          doOperation(stack, infixExpression[i], postfixExpression, &postfixExpressionLength);
        }

        /**
         * Pokud aktuální znak je pravá závorka,
         * zavoláme funkci untilLeftPar
         */
        if (infixExpression[i] == ')') {
           untilLeftPar(stack, postfixExpression, &postfixExpressionLength);
        }

        /**
         * Pokud aktuální znak je omezovač,
         * odstraníme prvky z vrcholu zásobíku
         * a přidáme je na konec řetězce
         */
        if (infixExpression[i] == '=') {
            char cTop;
            while (!Stack_IsEmpty(stack)) {
                Stack_Top(stack, &cTop);
                postfixExpression[postfixExpressionLength++] = cTop;
                Stack_Pop(stack);
            }

            postfixExpression[postfixExpressionLength] = '=';
            break;
        }
    }

    free(stack);
    return postfixExpression;
}

/* Konec c204.c */
