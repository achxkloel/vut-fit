/**
 * File:        process.h
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __PROCESS_H__ /* twice include guard */
#define __PROCESS_H__

#include <stdbool.h>
#include "proj2.h"

void proc_santa_action ();                                  /* Santa process actions */
void proc_elf_action (int id, int work_time);               /* Elf process actions */
void proc_rd_action (int id, int num, int return_time);     /* Reindeer process actions */
bool proc_init (proc_info_t *proc_info);                    /* Initialization of processes */

#endif /* __PROCESS_H__ */