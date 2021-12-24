/**
 * File:        eratosthenes.c
 * Project:     IJC-DU1, task a)
 * Author:      Evgeny Torbin
 * Date:        22.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 *
 * Description: sieve of Eratosthenes implementation
 */

#include <stdio.h>
#include "bitset.h"

/**
 * Sieve of Eratosthenes
 *
 * Calculate prime numbers.
 *
 * @param array
 * @return void
 */
void Eratosthenes (bitset_t array) {
    bitset_index_t start_from = 2;
    bitset_index_t array_size = bitset_size(array);

    /* Set 0 and 1 bit to 1, because these numbers are prime */
    bitset_setbit(array, 0, 1);
    bitset_setbit(array, 1, 1);

    for (bitset_index_t i = start_from; i * i < array_size; i++)
        if (!bitset_getbit(array, i))
            for (bitset_index_t j = i * i; j < array_size; j += i)
                bitset_setbit(array, j, 1);
}