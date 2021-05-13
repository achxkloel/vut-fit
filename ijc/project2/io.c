/**
 * File:        io.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdio.h>
#include <ctype.h>

/**
 * @name read_word
 *
 * @brief Read one word from the stream
 * Word is a sequence of different characters
 * separated by isspace characters.
 * Return amount af read symbols,
 * which could be more than max, if word is longer.
 *
 * @param s
 * @param max
 * @param f
 * @return int
 */
int read_word (char *s, int max, FILE *f)
{
    int c;
    int i = 0;
    int j = 0;

    if (max <= 0) return -1;

    while ((c = fgetc(f)) != EOF)
    {
        if (isspace(c))
        {
            if (i == 0) continue;
            else break;
        }

        if (i >= max - 1)
        {
            j++;
            continue;
        }

        s[i++] = c;
    }

    if (i == 0)
        return -1;

    if (i > 0)
        s[i] = '\0';

    return i + j;
}