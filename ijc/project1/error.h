/**
 * File:        error.h
 * Project:     IJC-DU1, task b)
 * Author:      Evgeny Torbin
 * Date:        22.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 */

#ifndef ERROR_H /* twice include guard */
#define ERROR_H

/* Print an error message to stderr in a specific format. */
void warning_msg (const char *fmt, ...);

/* Print an error message to stderr in a specific format and stop program. */
void error_exit (const char *fmt, ...);

#endif /* ERROR_H */