/**
 * File:        htab_clear.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <string.h>
#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_clear
 *
 * @brief Clear all items in hash table.
 *
 * @param t
 * @return void
 */
void htab_clear (htab_t *t)
{
    for (size_t i = 0; i < t->array_size; i++)
    {
        htab_item_t *item = t->array[i];
        htab_item_t *item_tmp;

        while(item != NULL)
        {
            free((char *) item->pair->key);
            free(item->pair);
            item_tmp = item->next;
            free(item);
            item = item_tmp;
        }

        t->array[i] = NULL;
    }

    t->items_size = 0;
}