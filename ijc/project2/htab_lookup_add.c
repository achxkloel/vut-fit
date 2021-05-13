/**
 * File:        htab_lookup_add.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <string.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_lookup_add
 *
 * @brief Find pair by the specified key.
 * Add new if item was not found
 * and return pointer to it.
 *
 * @param t
 * @param key
 * @return htab_pair_t *
 */
htab_pair_t * htab_lookup_add (htab_t *t, htab_key_t key)
{
    size_t array_index = (htab_hash_function(key) % t->array_size);
    htab_item_t *item = t->array[array_index];

    if (item == NULL)
    {
        htab_item_t *new_item = htab_create_item(key);
        if (new_item == NULL)
            return NULL;

        t->items_size++;
        t->array[array_index] = new_item;

        return new_item->pair;
    }

    htab_item_t * last_item;
    while (item != NULL)
    {
        if (strcmp(item->pair->key, key) == 0)
            return item->pair;

        last_item = item;
        item = item->next;
    }

    htab_item_t *new_item = htab_create_item(key);
    if (new_item == NULL)
        return NULL;

    t->items_size++;
    last_item->next = new_item;

    return new_item->pair;
}