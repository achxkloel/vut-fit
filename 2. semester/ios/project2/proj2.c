/**
 * File:        proj2.c
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "base.h"
#include "resource.h"
#include "process.h"
#include "proj2.h"

/* Global variables definition */
shared_data_t *shm_data;    /* Shared data */
int shm_id;                 /* Shared memory ID */

int main (int argc, char *argv[])
{
    /* Parse arguments */
    proc_info_t *proc_info = parse_args(argc, argv);
    if (proc_info == NULL) goto main_exit;

    /* Create shared memory */
    if (!shm_ctor()) goto main_free_info;

    /* Set shared memory variables */
    shm_data->total_count = 0;
    shm_data->awakened_by = ELF_WOKE_UP;
    shm_data->elf_help = 0;
    shm_data->rd_return = 0;
    shm_data->rd_hitched = 0;
    shm_data->elves_inside = 0;
    shm_data->workshop_closed = false;
    shm_data->end = false;

    /* Open the output file stream */
    shm_data->output_stream = fopen(OUTPUT_NAME, "w");
    if (shm_data->output_stream == NULL)
    {
        WARNING_MSG("failed to open %s", OUTPUT_NAME);
        goto main_shm_dtor;
    }

    /* Create semaphores */
    if (!sem_ctor()) goto main_fclose;

    /* Initialize processes */
    proc_init(proc_info);

    /* Waiting for all child processes */
    while(wait(NULL) >= 0);

    sem_dtor();                         /* Destruct semaphores */
    fclose(shm_data->output_stream);    /* Close the output file_stream */
    shm_dtor();                         /* Destruct shared memory */
    free(proc_info);                    /* Free proc_info structure */
    return 0;

    /* Error handlers */
    main_fclose:    fclose(shm_data->output_stream);
    main_shm_dtor:  shm_dtor();
    main_free_info: free(proc_info);
    main_exit:      return 1;
}
