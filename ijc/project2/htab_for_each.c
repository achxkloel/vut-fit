/**
 * File:        htab_for_each.c
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
 * @name htab_for_each
 *
 * @brief Goes throw all the elements
 * and on every of them call the function.
 *
 * @param t
 * @param f
 * @return void
 */
void htab_for_each (const htab_t *t, void (*f)(htab_pair_t *data))
{
    for (size_t i = 0; i < t->array_size; i++)
    {
        htab_item_t *item = t->array[i];

        while (item != NULL)
        {
            (*f)(item->pair);
            item = item->next;
        }
    }
}