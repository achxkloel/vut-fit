/**
 * File:        htab_hash_function.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdint.h>
#include "htab.h"

/**
 * @name htab_hash_function
 *
 * @brief Returns hash of the specified string.
 *
 * @param str
 * @return size_t
 */
size_t htab_hash_function (const char *str)
{
    uint32_t h = 0;
    const unsigned char *p;
    for (p = (const unsigned char *) str; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h;
}