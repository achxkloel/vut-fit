/**
 * File:        htab_print.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdio.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @name htab_print
 *
 * @brief Print pair (key => value)
 *
 * @param t
 * @return void
 */
void htab_print (htab_pair_t *data)
{
    printf("%s\t%d\n", data->key, data->value);
}