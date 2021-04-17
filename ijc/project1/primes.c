/**
 * File:        primes.c
 * Project:     IJC-DU1, task a)
 * Author:      Evgeny Torbin
 * Date:        23.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 *
 * Description: prints the last N primes up to a given number
 */

#include <stdio.h>
#include <time.h>
#include "eratosthenes.h"
#include "bitset.h"

/* Maximal number to count for */
#define MAX_NUMBER 200000000

/* Amount of last prime numbers */
#define PRIMARY_AMOUNT 10

int main (void)
{
    clock_t start = clock();

    bitset_create(p, MAX_NUMBER + 1);
    Eratosthenes(p);

    bitset_index_t result[PRIMARY_AMOUNT];
    int result_index = 0;
    bitset_index_t bitset_index = bitset_size(p) - 1;

    /* Write last N prime numbers */
    while (bitset_index >= 2 && result_index < PRIMARY_AMOUNT)
    {
        if (!bitset_getbit(p, bitset_index))
            result[result_index++] = bitset_index;

        bitset_index--;
    }

    /* Print prime numbers */
    for (int i = result_index - 1; i >= 0; i--)
        printf("%lu\n", result[i]);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}