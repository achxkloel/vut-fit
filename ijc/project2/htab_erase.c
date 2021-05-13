/**
 * File:        htab_erase.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_erase
 *
 * @brief Delete item by the specified key.
 * Return true if operation success,
 * false if failed.
 *
 * @param t
 * @param key
 * @return bool
 */
bool htab_erase (htab_t *t, htab_key_t key)
{
    size_t array_index = htab_hash_function(key) % t->array_size;
    htab_item_t *item = t->array[array_index];

    if (item == NULL)
        return false;

    if (strcmp(item->pair->key, key) == 0)
    {
        t->array[array_index] = item->next;
        t->items_size--;
        htab_free_item(item);
        return true;
    }

    htab_item_t *prev_item = item;
    item = item->next;
    while (item != NULL)
    {
        if (strcmp(item->pair->key, key) == 0)
        {
            prev_item->next = item->next;
            t->items_size--;
            htab_free_item(item);
            return true;
        }

        prev_item = item;
        item = item->next;
    }

    return false;
}