/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL || value == NULL) {
        return false;
    }

    bst_node_t *tmp = tree;

    while (tmp != NULL) {
        if (key < tmp->key) {               // Hledáme klič v levém podstromu
            tmp = tmp->left;
        } else if (key > tmp->key) {        // Hledáme klič v pravém podstromu
            tmp = tmp->right;
        } else {
            *value = tmp->value;
            return true;
        }
    }

    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    while (*tree != NULL) {
        if (key < (*tree)->key) {
            tree = &(*tree)->left;
        } else if (key > (*tree)->key) {
            tree = &(*tree)->right;
        } else {
            (*tree)->key = key;
            (*tree)->value = value;
            return;
        }
    }

    *tree = calloc(1, sizeof(bst_node_t));

    // Kontrola alokace
    if (*tree == NULL) {
        return;
    }

    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (target == NULL || *tree == NULL) {
        return;
    }

    while ((*tree)->right != NULL) {
        tree = &(*tree)->right;
    }

    target->key = (*tree)->key;
    target->value = (*tree)->value;
    bst_node_t *tmp = (*tree)->left;
    free(*tree);
    *tree = tmp;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    while (*tree != NULL) {
        if (key < (*tree)->key) {
            tree = &(*tree)->left;
        } else if (key > (*tree)->key) {
            tree = &(*tree)->right;
        } else {
            if ((*tree)->left != NULL && (*tree)->right != NULL) {          // Odstraněný uzel má oba podstromy
                bst_replace_by_rightmost((*tree), &(*tree)->left);
            } else if (((*tree)->left == NULL && (*tree)->right == NULL)) { // Odstráněný uzel nemá žadné podstromy
                free(*tree);
                bst_init(tree);
            } else {                                                        // Odstraněný uzel má pouze jeden podstrom
                bst_node_t *tmp;

                if ((*tree)->right != NULL) {
                    tmp = (*tree)->right;
                } else {
                    tmp = (*tree)->left;
                }

                free(*tree);
                *tree = tmp;
            }
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if (*tree == NULL) {
        return;
    }

    // Zásobnik uzlů
    stack_bst_t *stack = calloc(1, sizeof(stack_bst_t));

    // Kontrola alokace
    if (stack == NULL) {
        return;
    }

    // Inicializace zásobniku
    stack_bst_init(stack);

    // Uložíme root na vrchol zásobniku
    stack_bst_push(stack, *tree);

    while (!stack_bst_empty(stack)) {
        // Odebereme uzel z vrcholu zásobniku
        *tree = stack_bst_pop(stack);

        // Pokud existuje levý podstrom, uložíme ho na vrchol zásobniku
        if ((*tree)->left != NULL) {
            stack_bst_push(stack, (*tree)->left);
        }

        // Stejně s pravým podstromem
        if ((*tree)->right != NULL) {
            stack_bst_push(stack, (*tree)->right);
        }

        free(*tree);
        bst_init(tree);
    }

    // Uvolnění zásobniku
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *tmp = tree;

    while (tmp != NULL) {
        stack_bst_push(to_visit, tmp);
        bst_print_node(tmp);
        tmp = tmp->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    // Zásobnik uzlů
    stack_bst_t *stack = calloc(1, sizeof(stack_bst_t));

    // Kontrola alokace
    if (stack == NULL) {
        return;
    }

    stack_bst_init(stack);
    bst_node_t *tmp = tree;
    bst_leftmost_preorder(tmp, stack);

    while (!stack_bst_empty(stack)) {
        tmp = stack_bst_pop(stack);
        bst_leftmost_preorder(tmp->right, stack);
    }

    // Uvolnění zásobniku
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *tmp = tree;

    while (tmp != NULL) {
        stack_bst_push(to_visit, tmp);
        tmp = tmp->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t *stack = calloc(1, sizeof(stack_bst_t));

    if (stack == NULL) {
        return;
    }

    stack_bst_init(stack);
    bst_node_t *tmp = tree;
    bst_leftmost_inorder(tmp, stack);

    while (!stack_bst_empty(stack)) {
        tmp = stack_bst_pop(stack);
        bst_print_node(tmp);
        bst_leftmost_inorder(tmp->right, stack);
    }

    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    bst_node_t *tmp = tree;

    while (tmp != NULL) {
        stack_bst_push(to_visit, tmp);
        stack_bool_push(first_visit, true);
        tmp = tmp->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    // Zásobnik uzlů
    stack_bst_t *stack = calloc(1, sizeof(stack_bst_t));

    if (stack == NULL) {
        return;
    }

    // Zásobnik bool hodnot
    stack_bool_t *b_stack = calloc(1, sizeof(stack_bool_t));

    if (b_stack == NULL) {
        return;
    }

    stack_bst_init(stack);
    stack_bool_init(b_stack);
    bst_node_t *tmp = tree;
    bst_leftmost_postorder(tmp, stack, b_stack);

    while (!stack_bst_empty(stack)) {
        tmp = stack_bst_top(stack);

        if (stack_bool_pop(b_stack)) {                              // Přichází zleva
            stack_bool_push(b_stack, false);
            bst_leftmost_postorder(tmp->right, stack, b_stack);
        } else {                                                    // Přichází zprava, zpracuj uzel
            stack_bst_pop(stack);
            bst_print_node(tmp);
        }
    }

    // Uvolnění dvou zásobniků
    free(stack);
    free(b_stack);
}