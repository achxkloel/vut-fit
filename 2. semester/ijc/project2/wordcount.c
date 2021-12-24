/**
 * File:        wordcount.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        26.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "htab.h"
#include "io.h"

/* use own hash function if macro defined */
#ifdef HASHTEST

/* DJB2 (Daniel .J Bernstein's hashing algorithm) */
size_t htab_hash_function (const char *str)
{
    uint32_t h = 5381;
    const unsigned char *p;
    for (p = (const unsigned char *) str; *p != '\0'; p++)
        h = ((h << 5) + h) + *p; /* h * 33 + *p */
    return h;
}

#endif /* HASHTEST */

/* Maximal length of each word (150 characters + 1 null-terminated byte) */
#define MAX_LENGTH 151

// Pro zmenseni kolizi v hashovaci tabulce,
// rozhodl jsem vzit nahodne velke prvocislo.
// Taky to zvysuje rychlost programu.
#define HTAB_SIZE 65559

int main (void)
{
    htab_t *hash_table = htab_init(HTAB_SIZE);
    if (hash_table == NULL)
    {
        fprintf(stderr, "wordcount: hash table memory allocation error\n");
        return 1;
    }

    char string[MAX_LENGTH];
    int chars_count;
    bool max_flag = false;
    htab_pair_t *pair;

    while ((chars_count = read_word(string, MAX_LENGTH, stdin)) != EOF)
    {
        if (chars_count >= MAX_LENGTH)
        {
            if (!max_flag)
            {
                fprintf(stderr, "wordcount: word is too long\n");
                max_flag = true;
            }
        }

        pair = htab_lookup_add(hash_table, string);
        if (pair == NULL)
        {
            fprintf(stderr, "wordcount: pair memory allocation error\n");
            htab_free(hash_table);
            return 1;
        }
        pair->value++;
    }

    htab_for_each(hash_table, htab_print);

/* test htab_move function if macro defined */
#ifdef MOVETEST

/* Another size of hash table for testing */
#define NEW_HTAB_SIZE 39989

    htab_t *new_hash_table = htab_move(NEW_HTAB_SIZE, hash_table);
    if (new_hash_table == NULL)
    {
        fprintf(stderr, "wordcount: new hash table memory allocation error\n");
        htab_free(hash_table);
        return 1;
    }
    printf("=== MOVETEST START ===\n");
    htab_for_each(new_hash_table, htab_print);
    printf("==== MOVETEST END ====\n");
    htab_free(new_hash_table);

#endif /* MOVETEST */

    htab_free(hash_table);

    return 0;
}
