/**
 * File:        htab_find.c
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
 * @name htab_find
 *
 * @brief Find pair by the specified key.
 * Return NULL if item was not found.
 *
 * @param t
 * @param key
 * @return htab_pair_t *
 */
htab_pair_t * htab_find (htab_t *t, htab_key_t key)
{
    size_t array_index = htab_hash_function(key) % t->array_size;
    htab_item_t *item = t->array[array_index];

    while (item != NULL)
    {
        if (strcmp(item->pair->key, key) == 0)
            return item->pair;

        item = item->next;
    }

    return NULL;
}