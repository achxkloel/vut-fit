/**
 * File:        bitset.c
 * Project:     IJC-DU1, task a)
 * Author:      Evgeny Torbin
 * Date:        22.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 *
 * Description: array of bits implementation
 */

#include "bitset.h"

#ifdef USE_INLINE /* inline functions declaration */

/* Free allocated memory for array of bits. */
extern void bitset_free (bitset_t array);

/* Return size of array of bits. */
extern bitset_index_t bitset_size (bitset_t array);

/* Set the specified bit. */
extern void bitset_setbit (bitset_t array, bitset_index_t index, unsigned expr);

/* Get the specified bit. */
extern bitset_index_t bitset_getbit (bitset_t array, bitset_index_t index);

#endif /* USE_INLINE */