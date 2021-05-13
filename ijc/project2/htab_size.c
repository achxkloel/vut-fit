/**
 * File:        htab_size.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_size
 *
 * @brief Return amount of hash table items.
 *
 * @param t
 * @return size_t
 */
size_t htab_size (const htab_t *t)
{
    return t->items_size;
}