/**
 * File:        htab_create_item.c
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
 * @name htab_create_item
 *
 * @brief Creates new item.
 *
 * @param key
 * @return htab_item_t *
 */
htab_item_t * htab_create_item (htab_key_t key)
{
    htab_item_t *item = malloc(sizeof(htab_item_t));
    if (item == NULL)
        return NULL;

    item->pair = malloc(sizeof(htab_pair_t));
    if (item->pair == NULL)
    {
        free(item);
        return NULL;
    }

    item->pair->key = calloc(1, strlen(key) + 1);
    if (item->pair->key == NULL)
    {
        free(item->pair);
        free(item);
        return NULL;
    }

    strcpy((char *) item->pair->key, key);
    item->pair->value = 0;
    item->next = NULL;

    return item;
}