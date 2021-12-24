/**
 * File:        htab_free_item.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_free_item
 *
 * @brief Free allocated item.
 *
 * @param key
 * @return htab_item_t *
 */
void htab_free_item (htab_item_t *item)
{
    free((char *) item->pair->key);
    free(item->pair);
    free(item);
}