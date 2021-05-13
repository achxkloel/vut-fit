/**
 * File:        htab_move.c
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
 * @name htab_move
 *
 * @brief Create new hash table
 * and copy elements from old.
 * The old hash table will remain empty.
 *
 * @param n
 * @param from
 * @return htab_t *
 */
htab_t * htab_move (size_t n, htab_t *from)
{
    htab_t *to = htab_init(n);
    if (to == NULL)
        return NULL;

    for (size_t i = 0; i < from->array_size; i++)
    {
        htab_item_t *item = from->array[i];

        while (item != NULL)
        {
            htab_pair_t *pair = htab_lookup_add(to, item->pair->key);
            if (pair == NULL)
                return NULL;

            pair->value = item->pair->value;
            item = item->next;
        }
    }

    htab_clear(from);
    return to;
}