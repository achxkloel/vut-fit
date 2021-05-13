/**
 * File:        htab_init.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdlib.h>
#include <string.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_init
 *
 * @brief Initialize an empty hash table
 * with the specified size.
 *
 * @param n
 * @return htab_t *
 */
htab_t * htab_init (size_t n)
{
    htab_t *hash_table = malloc(sizeof(htab_t) + sizeof(htab_item_t *) * n);
    if (hash_table == NULL)
        return NULL;

    for (size_t i = 0; i < n; i++)
        hash_table->array[i] = NULL;

    hash_table->items_size = 0;
    hash_table->array_size = n;

    return hash_table;
}