/**
 * File:        process.c
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 * Description: process functions
 */

#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include "proj2.h"

/**
 * @name proc_santa_action
 *
 * @brief Santa process actions.
 *
 * @return void
 */
void proc_santa_action ()
{
    while (1)
    {
        /* Santa is going to sleep */
        sem_wait(&shm_data->sem_list.mutex);
        shm_data->total_count++;
        PROC_PRINT_MSG(shm_data->output_stream, "%d: Santa: going to sleep\n", shm_data->total_count);
        sem_post(&shm_data->sem_list.mutex);

        /* Waiting for a signal from elves or reindeer */
        sem_wait(&shm_data->sem_list.santa);

        /* Elves need help */
        if (shm_data->awakened_by == ELF_WOKE_UP)
        {
            /* Helping elves */
            sem_wait(&shm_data->sem_list.mutex);
            shm_data->total_count++;
            PROC_PRINT_MSG(shm_data->output_stream, "%d: Santa: helping elves\n", shm_data->total_count);
            sem_post(&shm_data->sem_list.mutex);

            for (int i = 0; i < ELF_INSIDE_MAX; i++)
                sem_post(&shm_data->sem_list.elf);

            /* Waiting until Elves leave workshop and going sleep */
            sem_wait(&shm_data->sem_list.helped);
        }
        /* Reindeer return from summer holiday */
        else if (shm_data->awakened_by == RD_WOKE_UP)
        {
            /* Closing workshop */
            sem_wait(&shm_data->sem_list.mutex);
            shm_data->total_count++;
            shm_data->workshop_closed = true;
            PROC_PRINT_MSG(shm_data->output_stream, "%d: Santa: closing workshop\n", shm_data->total_count);
            sem_post(&shm_data->sem_list.mutex);

            /* Signal Elves tha workshop is closed */
            sem_post(&shm_data->sem_list.elf);

            /* Santa hitch Reindeer and waiting a signal from them */
            sem_post(&shm_data->sem_list.rd);

            /* Start Christmas */
            sem_wait(&shm_data->sem_list.christmas);

            sem_wait(&shm_data->sem_list.mutex);
            shm_data->total_count++;
            PROC_PRINT_MSG(shm_data->output_stream, "%d: Santa: Christmas started\n", shm_data->total_count);
            sem_post(&shm_data->sem_list.mutex);

            exit(0);
        }
    }
}

/**
 * @name proc_elf_action
 *
 * @brief Elf process action.
 *
 * @param id
 * @param work_time
 * @return void
 */
void proc_elf_action (int id, int work_time)
{
    /* Set seed for each Elf process */
    SET_SEED(getpid());

    /* Elf started */
    sem_wait(&shm_data->sem_list.mutex);
    shm_data->total_count++;
    PROC_PRINT_MSG(shm_data->output_stream, "%d: Elf %d: started\n", shm_data->total_count, id);
    sem_post(&shm_data->sem_list.mutex);

    while (1) {
        /* Elf work simulation */
        usleep(RAND_NUM(0, work_time));

        /* Elf need help */
        sem_wait(&shm_data->sem_list.mutex);
        shm_data->total_count++;
        shm_data->elf_help++;
        PROC_PRINT_MSG(shm_data->output_stream, "%d: Elf %d: need help\n", shm_data->total_count, id);
        if (shm_data->elf_help >= ELF_INSIDE_MAX && shm_data->elves_inside == 0) {
            shm_data->elves_inside = ELF_INSIDE_MAX;
            sem_post(&shm_data->sem_list.santa);
        }
        sem_post(&shm_data->sem_list.mutex);

        /* Waiting for Santa's signal */
        sem_wait(&shm_data->sem_list.elf);

        /* If Santa was not created */
        if (shm_data->end == true)
        {
            sem_post(&shm_data->sem_list.elf);
            exit(0);
        }

        /* Taking holidays if workshop closed */
        if (shm_data->workshop_closed)
        {
            sem_wait(&shm_data->sem_list.mutex);
            shm_data->total_count++;
            PROC_PRINT_MSG(shm_data->output_stream, "%d: Elf %d: taking holidays\n", shm_data->total_count, id);
            sem_post(&shm_data->sem_list.mutex);
            sem_post(&shm_data->sem_list.elf);
            exit(0);
        }

        /* Elf get help */
        sem_wait(&shm_data->sem_list.mutex);
        shm_data->total_count++;
        shm_data->elf_help--;
        shm_data->elves_inside--;
        PROC_PRINT_MSG(shm_data->output_stream, "%d: Elf %d: get help\n", shm_data->total_count, id);
        if (shm_data->elves_inside == 0)
            sem_post(&shm_data->sem_list.helped);
        sem_post(&shm_data->sem_list.mutex);
    }
}

/**
 * @name proc_rd_action
 *
 * @brief Reindeer process action.
 *
 * @param id
 * @param num
 * @param return_time
 * @return void
 */
void proc_rd_action (int id, int num, int return_time)
{
    /* Set seed for each Reindeer process */
    SET_SEED(getpid());

    /* Reindeer start */
    sem_wait(&shm_data->sem_list.mutex);
    shm_data->total_count++;
    PROC_PRINT_MSG(shm_data->output_stream, "%d: RD %d: rstarted\n", shm_data->total_count, id);
    sem_post(&shm_data->sem_list.mutex);

    /* Reindeer return simulation */
    usleep(RAND_NUM(return_time >> 1, return_time));

    /* Return home */
    sem_wait(&shm_data->sem_list.mutex);
    shm_data->total_count++;
    shm_data->rd_return++;
    PROC_PRINT_MSG(shm_data->output_stream, "%d: RD %d: return home\n", shm_data->total_count, id);
    if (shm_data->rd_return == num)
    {
        shm_data->awakened_by = RD_WOKE_UP;
        sem_post(&shm_data->sem_list.santa);
    }
    sem_post(&shm_data->sem_list.mutex);

    /* Waiting for Santa's signal */
    sem_wait(&shm_data->sem_list.rd);

    /* If Santa was not created */
    if (shm_data->end == true)
    {
        sem_post(&shm_data->sem_list.rd);
        exit(0);
    }

    /* Hitching Reindeer */
    sem_wait(&shm_data->sem_list.mutex);
    shm_data->total_count++;
    shm_data->rd_hitched++;
    PROC_PRINT_MSG(shm_data->output_stream, "%d: RD %d: get hitched\n", shm_data->total_count, id);
    if (shm_data->rd_hitched == num)
        sem_post(&shm_data->sem_list.christmas);
    sem_post(&shm_data->sem_list.mutex);

    /* Next Reindeer to hitch */
    sem_post(&shm_data->sem_list.rd);

    exit(0);
}

/**
 * @name proc_init
 *
 * @brief Initialization of processes.
 *
 * @param proc_info
 * @return void
 */
bool proc_init (proc_info_t *proc_info)
{
    /* Process Santa */
    int proc_santa = fork();
    if (proc_santa == -1)
    {
        WARNING_MSG("failed to create process Santa");

        sem_wait(&shm_data->sem_list.mutex);
        shm_data->end = true;
        sem_post(&shm_data->sem_list.mutex);

        sem_post(&shm_data->sem_list.rd);
        sem_post(&shm_data->sem_list.elf);
    }
    else if (proc_santa == 0)
    {
        proc_santa_action();
    }

    /* Processes Elves */
    for (int i = 0; i < proc_info->elf_number; i++)
    {
        int proc_elf = fork();
        if (proc_elf == -1)
        {
            WARNING_MSG("failed to create process Elf");
        }
        else if (proc_elf == 0)
        {
            proc_elf_action(i + 1, proc_info->elf_time);
        }
    }

    /* Reindeer processes */
    for (int i = 0; i < proc_info->rd_number; i++)
    {
        int proc_rd = fork();
        if (proc_rd == -1)
        {
            WARNING_MSG("failed to create process Reindeer");
        }
        else if (proc_rd == 0)
        {
            proc_rd_action(i + 1, proc_info->rd_number, proc_info->rd_time);
        }
    }

    return true;
}