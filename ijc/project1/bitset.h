/**
 * File:        bitset.h
 * Project:     IJC-DU1, task a)
 * Author:      Evgeny Torbin
 * Date:        25.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 */

#ifndef BITSET_H /* twice include guard */
#define BITSET_H

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"

/* Type of array of bits */
typedef unsigned long *bitset_t;

/* Type of index in array of bits */
typedef unsigned long bitset_index_t;

/* Maximum size of array of bits */
#define BITSET_LIMIT 200000001

/* Amount of bytes in bitset_index_t type */
#define BITSET_BYTES sizeof(bitset_index_t)

/* Amount of bits in bitset_index_t type */
#define BITSET_BITS (BITSET_BYTES * CHAR_BIT)

/**
 * Bitset getpos
 *
 * Get position of the specified bit
 * in one item of array.
 *
 * Uses bit index.
 *
 * @param index
 */
#define bitset_getpos(index) \
        (index % BITSET_BITS)

/**
 * Bitset indices
 *
 * Counts the number of indices for
 * array of bits.
 *
 * @param size
 */
#define bitset_indices(size) \
        ( (size) / BITSET_BITS + (bitset_getpos( (size) ) ? 1 : 0) + 1 )

/**
 * Bitset create
 *
 * Statically creates an array of bits of the specified size.
 * The first element of this array is its size.
 *
 * @param array
 * @param size
 */
#define bitset_create(array, size) \
        bitset_index_t (array)[bitset_indices( (size) )] = { (size) }; \
        static_assert( (size) > 0 && (size) <= BITSET_LIMIT, "bitset_create: Wrong size of the bit array" );

/**
 * Bitset alloc
 *
 * Dynamically creates an array of bits of the specified size.
 * The first element of this array is its size.
 *
 * @param array
 * @param size
 */
#define bitset_alloc(array, size) \
        assert( (size) > 0 && (size) <= BITSET_LIMIT && "bitset_alloc: Wrong size of the bit array" ); \
        bitset_t (array) = calloc(bitset_indices( (size) ), BITSET_BYTES); \
        if ( (array) == NULL ) error_exit("bitset_alloc: Chyba alokace paměti\n"); \
        (array)[0] = size;

#ifndef USE_INLINE /* define other macros */

/**
 * Bitset free
 *
 * Free allocated memory for array of bits.
 *
 * @param array
 */
#define bitset_free(array) \
        free ( (array) );

/**
 * Bitset size
 *
 * Return size of array of bits.
 *
 * @param array
 */
#define bitset_size(array) \
        (array)[0]

/**
 * Bitset setbit
 *
 * Set the specified bit.
 * Zero expression -> set bit to 0.
 * Nonzero expression -> set bit to 1.
 *
 * @param array
 * @param index
 * @param expr
 */
#define bitset_setbit(array, index, expr) \
        do { \
            if ( (index) >= bitset_size( (array) ) ) \
            error_exit( \
                "bitset_setbit: Index %lu mimo rozsah 0..%lu\n", \
                (bitset_index_t) (index), \
                (bitset_index_t) bitset_size( (array) ) - 1 \
            ); \
            if (expr) (array)[bitset_indices( (index) + 1 ) - 1] |= 1L << bitset_getpos( (index) ); \
            else array[bitset_indices( (index) + 1 ) - 1] &= ~(1L << bitset_getpos( (index) ));\
        } while (0)

/**
 * Bitset getbit
 *
 * Get the specified bit.
 *
 * @param array
 * @param index
 */
#define bitset_getbit(array, index) \
        (\
            ( (index) >= bitset_size( (array) ) ) ? \
                (error_exit( \
                    "bitset_getbit: Index %lu mimo rozsah 0..%lu\n", \
                    (bitset_index_t) (index), \
                    (bitset_index_t) bitset_size( (array) ) - 1 \
                ), 1) : \
                (array[bitset_indices( (index) + 1 ) - 1] >> bitset_getpos( (index) ) & 1UL) \
        )

#else /* define inline functions */

/**
 * Bitset free inline
 *
 * @param array
 * @return void
 */
inline void bitset_free (bitset_t array)
{
    free(array);
}

/**
 * Bitset size inline
 *
 * @param array
 * @return bitset_index_t
 */
inline bitset_index_t bitset_size (bitset_t array)
{
    return array[0];
}

/**
 * Bitset setbit inline
 *
 * @param array
 * @param index
 * @param expr
 * @return void
 */
inline void bitset_setbit (bitset_t array, bitset_index_t index, unsigned expr)
{
    if (index >= bitset_size(array))
        error_exit(
            "%s: Index %lu mimo rozsah 0..%lu\n",
            __func__,
            (bitset_index_t) index,
            (bitset_index_t) bitset_size(array) - 1
        );
    if (expr) array[bitset_indices(index + 1) - 1] |= 1UL << (bitset_getpos(index));
    else array[bitset_indices(index + 1) - 1] &= ~(1UL << (bitset_getpos(index)));
}

/**
 * Bitset getbit inline
 *
 * @param array
 * @param index
 * @return bitset_index_t
 */
inline bitset_index_t bitset_getbit (bitset_t array, bitset_index_t index)
{
    if (index >= bitset_size(array))
        error_exit(
            "%s: Index %lu mimo rozsah 0..%lu\n",
            __func__,
            (bitset_index_t) index,
            (bitset_index_t) bitset_size(array) - 1
        );
    return array[bitset_indices(index + 1) - 1] >> (bitset_getpos(index)) & 1UL;
}

#endif /* USE_INLINE */
#endif /* BITSET_H */