/**
 * File:        resource.c
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 * Description: shared memory and semaphore functions
 */

#include <stdbool.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "proj2.h"

/**
 * @name shm_ctor
 *
 * @brief Create shared memory (allocate and attach).
 * Returns true if creation was successful.
 *
 * @return bool
 */
bool shm_ctor ()
{
    key_t ipc_key = ftok("proj2.c", 'a');
    if (ipc_key == (key_t) -1)
    {
        WARNING_MSG("failed to create IPC key");
        return false;
    }

    shm_id = shmget(ipc_key, sizeof(shared_data_t), IPC_CREAT | SHM_PERM);
    if (shm_id < 0)
    {
        WARNING_MSG("failed to allocate shared memory");
        return false;
    }

    shm_data = shmat(shm_id, NULL, 0);
    if (shm_data == (shared_data_t *) -1)
    {
        WARNING_MSG("failed to attach shared memory");
        return false;
    }

    return true;
}

/**
 * @name shm_dtor
 *
 * @brief Destruct shared memory (detach and destroy).
 *
 * @return void
 */
void shm_dtor ()
{
    if (shmdt(shm_data) == -1)
        WARNING_MSG("failed to detach shared memory");

    if (shmctl(shm_id, IPC_RMID, NULL) == -1)
        WARNING_MSG("failed to destroy shared memory");
}

/**
 * @name sem_ctor
 *
 * @brief Initialize semaphores.
 * Returns true if initialization was successful.
 *
 * @return bool
 */
bool sem_ctor ()
{
    if (sem_init(&shm_data->sem_list.mutex, 1, 1) == -1)
    {
        WARNING_MSG("failed to initialize semaphore mutex");
        return false;
    }

    if (sem_init(&shm_data->sem_list.santa, 1, 0) == -1)
    {
        WARNING_MSG("failed to initialize semaphore santa");
        return false;
    }

    if (sem_init(&shm_data->sem_list.elf, 1, 0) == -1)
    {
        WARNING_MSG("failed to initialize semaphore elf");
        return false;
    }

    if (sem_init(&shm_data->sem_list.rd, 1, 0) == -1)
    {
        WARNING_MSG("failed to initialize semaphore rd");
        return false;
    }

    if (sem_init(&shm_data->sem_list.christmas, 1, 0) == -1)
    {
        WARNING_MSG("failed to initialize semaphore christmas");
        return false;
    }

    if (sem_init(&shm_data->sem_list.helped, 1, 0) == -1)
    {
        WARNING_MSG("failed to initialize semaphore helped");
        return false;
    }

    return true;
}

/**
 * @name sem_dtor
 *
 * @brief Destruct semaphores (destroy).
 *
 * @return void
 */
void sem_dtor ()
{
    if (sem_destroy(&shm_data->sem_list.mutex) == -1)
        WARNING_MSG("failed to destroy semaphore mutex");

    if (sem_destroy(&shm_data->sem_list.santa) == -1)
        WARNING_MSG("failed to destroy semaphore santa");

    if (sem_destroy(&shm_data->sem_list.elf) == -1)
        WARNING_MSG("failed to destroy semaphore elf");

    if (sem_destroy(&shm_data->sem_list.rd) == -1)
        WARNING_MSG("failed to destroy semaphore rd");

    if (sem_destroy(&shm_data->sem_list.christmas) == -1)
        WARNING_MSG("failed to destroy semaphore christmas");

    if (sem_destroy(&shm_data->sem_list.helped) == -1)
        WARNING_MSG("failed to destroy semaphore helped");
}