/**
 * File:        base.h
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __BASE_H__ /* twice include guard */
#define __BASE_H__

#include <stdbool.h>
#include "proj2.h"

bool str_to_int (const char *str, int *output);         /* convert string to integer */
proc_info_t * parse_args (int argc, char *argv[]);      /* parse command line arguments */

#endif /* __BASE_H__ */