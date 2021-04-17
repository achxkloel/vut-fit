/**
 * File:        error.c
 * Project:     IJC-DU1, task b)
 * Author:      Evgeny Torbin
 * Date:        22.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 *
 * Description: error output
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * Warning message function
 *
 * Print an error message to stderr
 * in a specific format.
 *
 * @param fmt
 * @param ...
 * @return void
 */
void warning_msg (const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, args);

    va_end(args);
}

/**
 * Error exit function
 *
 * Print an error message to stderr
 * in a specific format and stop program.
 *
 * @param fmt
 * @param ...
 * @return void
 */
void error_exit (const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, args);

    va_end(args);
    exit(1);
}