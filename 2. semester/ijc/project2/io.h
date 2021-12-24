/**
 * File:        io.c
 * Project:     IJC-DU2, task 2)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __IO_H__ /* twice include guard */
#define __IO_H__

#include <stdio.h>

/* Read one word from the stream */
int read_word (char *s, int max, FILE *f);

#endif /* __IO_H__ */