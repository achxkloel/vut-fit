/**
 * File:        steg-decode.c
 * Project:     IJC-DU1, task b)
 * Author:      Evgeny Torbin
 * Date:        25.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 *
 * Description: decode a secret message from ppm file
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ppm.h"
#include "error.h"
#include "bitset.h"
#include "eratosthenes.h"

int main (int argc, char *argv[])
{
    /* Check arguments count */
    if (argc < 2)
        error_exit("%s: No such arguments\n", __func__);

    /* Read PPM data */
    struct ppm *image = ppm_read(argv[1]);
    if (image == NULL)
        error_exit("%s: Could not read PPM file\n", __func__);

    unsigned long data_size = image->xsize * image->ysize * 3;

    bitset_alloc(bitfield, data_size + 1);
    Eratosthenes(bitfield);

    char *msg = calloc(1, 1);
    if (msg == NULL)
    {
        bitset_free(bitfield);
        ppm_free(image);
        error_exit("%s: Memory allocation error\n", __func__);
    }

    char *msg_p;
    unsigned long msg_length = 0;
    char ch = 0;
    unsigned shift = 0;
    /* null terminated byte */
    bool ntb = false;

    /* Decode secret message */
    for (bitset_index_t i = 23; i < data_size + 1; i++)
    {
        if (!bitset_getbit(bitfield, i))
        {
            ch |= (image->data[i] & 1) << shift;
            shift++;

            if (shift == 8)
            {
                if (ch == '\0')
                {
                    ntb = true;
                    break;
                }

                msg_length++;
                msg_p = realloc(msg, msg_length + 1);
                if (msg_p == NULL)
                {
                    free(msg);
                    bitset_free(bitfield);
                    ppm_free(image);
                    error_exit("%s: Memory allocation error\n", __func__);
                }

                msg = msg_p;
                memset(msg + msg_length, 0, 1);

                /* Add char to message*/
                msg[msg_length - 1] = ch;
                ch = shift = 0;
            }
        }
    }

    /* Check on null-terminated byte */
    if (!ntb)
    {
        free(msg);
        bitset_free(bitfield);
        ppm_free(image);
        error_exit("%s: There is no null-terminated byte at the end of message\n", __func__);
    }

    /* Print result message */
    printf("%s\n", msg);

    /* Free allocated memory */
    free(msg);
    bitset_free(bitfield);
    ppm_free(image);

    return 0;
}