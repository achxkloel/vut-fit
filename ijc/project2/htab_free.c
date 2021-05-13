/**
 * File:        htab_free.c
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
 * @name htab_free
 *
 * @brief Free allocated hash table.
 *
 * @param t
 * @return void
 */
void htab_free (htab_t *t)
{
    htab_clear(t);
    free(t);
}