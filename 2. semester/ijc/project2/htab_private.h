/**
 * File:        htab_private.h
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __HTAB_PRIVATE_H__ /* twice include guard */
#define __HTAB_PRIVATE_H__

#include <string.h>
#include "htab.h"

/* Structure of hash table item */
struct htab_item {
    htab_pair_t         *pair;
    struct htab_item    *next;
};

/* htab_item structure type definition */
typedef struct htab_item htab_item_t;

/* Structure of hash table */
struct htab {
    size_t          items_size;
    size_t          array_size;
    htab_item_t     *array[];
};

/* Creates new hash table item */
htab_item_t * htab_create_item(htab_key_t key);

/* Free allocated item */
void htab_free_item(htab_item_t *item);

/* Debug print of hash table */
void htab_debug_print(htab_t *t);

#endif /* __HTAB_PRIVATE_H__ */