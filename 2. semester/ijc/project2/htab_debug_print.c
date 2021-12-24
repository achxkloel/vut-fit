/**
 * File:        htab_debug_print.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdio.h>
#include <string.h>
#include "htab.h"
#include "htab_private.h"

#define START_BLOCK     "==== DEBUG  START ===="
#define END_BLOCK       "===== DEBUG  END ====="
#define UPPER_BOUND     ">--------------------<"
#define LOWER_BOUND     ">--------------------<"
#define OPEN_BRACKET    "{"
#define CLOSE_BRACKET   "}"
#define PREFIX          ">"

/**
 * @name htab_debug_print
 *
 * @brief Print hash table.
 *
 * @param t
 * @return void
 */
void htab_debug_print (htab_t *t)
{
    printf("\n%s\n", START_BLOCK);
    printf("items->size = %lu\n", t->items_size);
    printf("array->size = %lu\n", t->array_size);

    for (size_t i = 0; i < t->array_size; i++)
    {
        printf("\n%s\n", UPPER_BOUND);
        printf("%s array[%lu] =", PREFIX, i);
        htab_item_t *item = t->array[i];

        if (item == NULL)
        {
            printf(" NULL\n");
            printf("%s\n", LOWER_BOUND);
            continue;
        }
        else
        {
            printf("\n%s %s\n", PREFIX, OPEN_BRACKET);
        }

        while (item != NULL)
        {
            printf("%s \t%s => %d\n", PREFIX, item->pair->key, item->pair->value);
            item = item->next;
        }

        printf("%s %s\n", PREFIX, CLOSE_BRACKET);
        printf("%s\n", LOWER_BOUND);
    }

    printf("%s\n\n", END_BLOCK);
}