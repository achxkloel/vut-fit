/**
 * File:        htab_bucket_count.c
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
 * @name htab_bucket_count
 *
 * @brief Return amount of hash table
 * array elements.
 *
 * @param t
 * @return size_t
 */
size_t htab_bucket_count (const htab_t *t)
{
    return t->array_size;
}