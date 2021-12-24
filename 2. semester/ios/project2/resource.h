/**
 * File:        resource.h
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __RESOURCE_H__ /* twice include guard */
#define __RESOURCE_H__

#include <stdbool.h>

bool shm_ctor ();   /* create shared memory */
void shm_dtor ();   /* destruct shared memory */
bool sem_ctor ();   /* initialize semaphores */
void sem_dtor ();   /* destruct semaphores */

#endif /* __RESOURCE_H__ */