/**
 * File:        base.c
 * Project:     IOS-2 (synchronization)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        02.05.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 * Description: base functions
 */

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include "proj2.h"

/**
 * @name str_to_int
 *
 * @brief Convert string to int.
 * Set result to output variable.
 * Returns false if error.
 *
 * @param str
 * @param output
 * @return bool
 */
bool str_to_int (const char *str, int *output)
{
    if (*str == '\0' || isspace(*str))
    {
        WARNING_MSG("number is empty or begins with whitespace");
        return false;
    }

    char *r;
    int base = 10;
    long num;

    num = strtol(str, &r, base);
    if (*r != '\0')
    {
        WARNING_MSG("number contains invalid characters");
        return false;
    }

    if (num < INT_MIN || num > INT_MAX)
    {
        WARNING_MSG("number is too long");
        return false;
    }

    *output = num;
    return true;
}

/**
 * @name parse_args
 *
 * @brief Parse command line arguments
 * and set them to a structure.
 * Returns NULL if error.
 *
 * @param argc
 * @param argv
 * @return proc_info_t *
 */
proc_info_t * parse_args (int argc, char *argv[])
{
    if (argc != 5)
    {
        WARNING_MSG("invalid number of arguments");
        goto parse_exit;
    }

    proc_info_t *proc_info = malloc(sizeof(proc_info_t));
    if (proc_info == NULL)
    {
        WARNING_MSG("failed to allocate memory for process info structure");
        goto parse_exit;
    }

    if (!str_to_int(argv[1], &proc_info->elf_number) ||
        !str_to_int(argv[2], &proc_info->rd_number) ||
        !str_to_int(argv[3], &proc_info->elf_time) ||
        !str_to_int(argv[4], &proc_info->rd_time))
    {
        WARNING_MSG("all arguments must be numbers");
        goto parse_free_info;
    }

    bool intervals_error = false;

    if (proc_info->elf_number < 1 || proc_info->elf_number > MAX_ELF_NUM)
    {
        WARNING_MSG("the number of elves must be between 1 and %d", MAX_ELF_NUM);
        intervals_error = true;
    }

    if (proc_info->rd_number < 1 || proc_info->rd_number > MAX_RD_NUM)
    {
        WARNING_MSG("the number of reindeer must be between 1 and %d", MAX_RD_NUM);
        intervals_error = true;
    }

    if (proc_info->elf_time < 0 || proc_info->elf_time > MAX_ELF_TIME)
    {
        WARNING_MSG("elf work time must be between 0 and %d", MAX_ELF_TIME);
        intervals_error = true;
    }

    if (proc_info->rd_time < 0 || proc_info->rd_time > MAX_RD_TIME)
    {
        WARNING_MSG("reindeer return time must be between 0 and %d", MAX_RD_TIME);
        intervals_error = true;
    }

    if (intervals_error) goto parse_free_info;

    return proc_info;

    /* Error handlers */
    parse_free_info:    free(proc_info);
    parse_exit:         return NULL;
}