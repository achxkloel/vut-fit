/**
 * File:        htab.h
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __HTAB_H__ /* twice include guard */
#define __HTAB_H__

#include <stdio.h>
#include <string.h> /* size_t */
#include <stdbool.h> /* bool */

/* Hash table structure - incomplete declaration */
struct htab;

/* htab structure type definition (according to the task) */
typedef struct htab htab_t;

/* Types */
typedef const char * htab_key_t;        /* key type */
typedef int htab_value_t;               /* value type */

/* Hash table pair structure */
typedef struct htab_pair {
    htab_key_t    key;                  /* key */
    htab_value_t  value;                /* value */
} htab_pair_t;                          /* type definition (according to the task) */

/* Function to distribute elements across the hash table */
size_t htab_hash_function (htab_key_t str);

/* Functions for working with a table */
htab_t *htab_init (size_t n);                   /* hash table constructor */
htab_t *htab_move (size_t n, htab_t *from);     /* move data to a new hash table */
size_t htab_size (const htab_t *t);             /* amount of items in hash table */
size_t htab_bucket_count (const htab_t *t);     /* amount of pointers in hash table*/

htab_pair_t * htab_find (htab_t * t, htab_key_t key);           /* searching a pair */
htab_pair_t * htab_lookup_add (htab_t * t, htab_key_t key);     /* searching and adding a pair */

bool htab_erase (htab_t * t, htab_key_t key);   /* delete item */

/* Loop throw all of items and call a function on them */
void htab_for_each (const htab_t * t, void (*f)(htab_pair_t *data));

void htab_clear (htab_t * t);           /* delete all items */
void htab_free (htab_t * t);            /* free allocated hash table */
void htab_print (htab_pair_t *data);    /* print item */

#endif /* __HTAB_H__ */