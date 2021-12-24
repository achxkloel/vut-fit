/**
 * File:        proj2.h
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#ifndef __PROJ2_H__ /* twice include guard */
#define __PROJ2_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define SHM_PERM        0666    /* Shared memory permissions */

#define MAX_ELF_NUM     1000    /* Maximal number of Elf processes */
#define MAX_RD_NUM      20      /* Maximal number of Reindeer processes */
#define MAX_ELF_TIME    1000    /* Maximal work time of Elf processes */
#define MAX_RD_TIME     1000    /* Maximal return time of Reindeer processes */

#define OUTPUT_NAME     "proj2.out"     /* Output filename */

#define ELF_INSIDE_MAX  3       /* Maximal number of elves in workshop */

/* Prints error message */
#define WARNING_MSG(...) \
    do \
    { \
        fprintf(stderr, "proj2: %s(%d): ", __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)

/* Prints process action message */
#define PROC_PRINT_MSG(f, ...) \
    do \
    { \
        fprintf( (f), __VA_ARGS__ ); \
        fflush( (f) ); \
    } while (0)

/* Set seed for rand function */
#define SET_SEED(num) srand( time(NULL) ^ (num) )

/* Count random number on the specified interval */
#define RAND_NUM(from, to) ( rand() % ( (to) - (from) + 1 ) + (from) )

/* Process info structure */
typedef struct ProcInfo
{
    int elf_number;         /* Number of Elves */
    int rd_number;          /* Number of Reindeer */
    int elf_time;           /* Work time of Elves */
    int rd_time;            /* Return time of Reindeer */
} proc_info_t;

/* Semaphore list structure */
typedef struct SemaphoreList
{
    sem_t mutex;            /* safety writing */
    sem_t santa;            /* awake Santa */
    sem_t elf;              /* interaction between Santa and Elves */
    sem_t rd;               /* interaction between Santa and Reindeer */
    sem_t christmas;        /* starting Christmas */
    sem_t helped;           /* Elves got help */
} semaphore_list_t;

/* Everyone, who can wake up Santa */
enum AwakenedBy { ELF_WOKE_UP, RD_WOKE_UP };

/* Shared data structure */
typedef struct SharedData
{
    int total_count;        /* actions counter */
    int awakened_by;        /* who woke up Santa */
    int elf_help;           /* Elves in queue counter */
    int rd_return;          /* returned Reindeer counter */
    int rd_hitched;         /* hitched Reindeer counter */
    int elves_inside;       /* Elves inside workshop value */
    bool workshop_closed;   /* workshop status */
    bool end;

    FILE *output_stream;        /* output file */
    semaphore_list_t sem_list;  /* semaphores list */
} shared_data_t;

/* Global variables declaration */
extern shared_data_t *shm_data; /* Shared data */
extern int shm_id;              /* Shared memory ID */

#endif /* __PROJ2_H__ */