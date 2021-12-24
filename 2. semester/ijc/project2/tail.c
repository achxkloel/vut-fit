/**
 * File:        tail.c
 * Project:     IJC-DU2, task 1)
 * Author:      Evgeny Torbin
 * Faculty:     Information Technology, Brno University of Technology
 * Date:        19.04.2021
 * Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

/* Maximal length of one row (500 characters + \n and \0) */
#define MAX_ROW_LENGTH 502

/* Absolute value operation */
#define ABS(x) ( (x) < 0 ) ? -(x) : (x)

/* Warning message */
#define WARNING_MSG(...) \
    do \
    { \
        fprintf(stderr, "tail: %s(%d): ", __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
    } while (0)

enum Stream     { FILE_INPUT, STDIN };          /* Stream list */
enum Type       { FROM_TOP, FROM_BOTTOM };      /* List of possible types */
enum ErrorCodes { FAILURE, SUCCESS };           /* List of error codes */

/* Tail parameters structure */
typedef struct Options {
    int     stream;
    char    *filename;
    int     row_count;
    int     type;
} tail_t;

/* Type definition of circular buffer */
typedef char data_field;

/* Circular buffer structure */
typedef struct Data {
    int     start;
    int     end;
    int     size;
    bool    is_full;
    bool    is_empty;
    data_field *buffer[];
} tail_data_t;

/**
 * @name get_line
 *
 * @brief Read one line from the stream.
 * The line always ends with newline character.
 * Return amount af read symbols,
 * which could be more than max, if line is longer.
 *
 * @param str
 * @param n
 * @param stream
 * @return int
 */
int get_line (char *str, int n, FILE *stream)
{
    int c;
    int read_count   = 0;
    int skip_count   = 0;
    bool eol_flag    = false;

    if (n <= 0) return -1;

    while ((c = fgetc(stream)) != EOF)
    {
        if (read_count >= n - 1) skip_count++;
        else str[read_count++] = c;

        if (c == '\n')
        {
            if (!skip_count) eol_flag = 1;
            break;
        }
    }

    if (read_count == 0) return -1;
    str[read_count] = '\0';
    if (!eol_flag && c != EOF) str[read_count - 1] = '\n';

    return read_count + skip_count;
}

/**
 * @name options_ctor
 *
 * @brief Create options structure
 * with default parameters.
 *
 * @return tail_t
 */
tail_t options_ctor()
{
    tail_t options =
    {
        .stream     = STDIN,
        .filename   = "",
        .row_count  = 10,
        .type       = FROM_BOTTOM
    };

    return options;
}

/**
 * @name data_ctor
 *
 * @brief Create circular buffer structure
 * and set default parameters.
 *
 * @param size
 * @return tail_data_t *
 */
tail_data_t * data_ctor (int size)
{
    tail_data_t *data = malloc(sizeof(tail_data_t) + sizeof(data_field *) * size);
    if (data == NULL)
    {
        WARNING_MSG("memory allocation error\n");
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        data->buffer[i] = malloc(MAX_ROW_LENGTH);
        if (data->buffer[i] == NULL)
        {
            free(data);
            WARNING_MSG("memory allocation error\n");
            return NULL;
        }
    }

    data->size      = size;
    data->start     = 0;
    data->end       = data->start;
    data->is_full   = false;
    data->is_empty  = true;

    return data;
}

/**
 * @name data_write
 *
 * @brief Write the specified row to
 * the circular buffer.
 *
 * @param data
 * @param row
 * @return void
 */
void data_write (tail_data_t *data, char *row)
{
    if (!data->is_full)
    {
        if (data->is_empty) data->is_empty = false;
        strcpy(data->buffer[data->end], row);

        if (data->end == data->size - 1)
        {
            data->is_full   = 1;
            data->end       = data->start;
            return;
        }

        data->end++;
    }
    else
    {
        strcpy(data->buffer[data->start], row);
        data->start = (data->start + 1) % data->size;
        data->end = data->start;
    }
}

/**
 * @name data_print
 *
 * @brief Print circular buffer.
 *
 * @param data
 * @return void
 */
void data_print (tail_data_t * data)
{
    if (!data->is_empty)
    {
        int index = data->start;

        do
        {
            printf("%s", data->buffer[index]);
            index = (index + 1) % data->size;
        }
        while (index != data->end);
    }
}

/**
 * @name data_dtor
 *
 * @brief Free allocated circular buffer structure
 *
 * @param data
 * @return void
 */
void data_dtor (tail_data_t * data)
{
    for (int i = 0; i < data->size; i++)
        free(data->buffer[i]);

    free(data);
}

/**
 * @name str_to_int
 *
 * @brief Convert string to integer.
 *
 * @param str
 * @param output
 * @return int
 */
int str_to_int (const char *str, int *output)
{
    if (*str == '\0' || isspace(*str))
    {
        WARNING_MSG("number is empty or begins with whitespace\n");
        return FAILURE;
    }

    char *r;
    int base = 10;
    long num;

    num = strtol(str, &r, base);
    if (*r != '\0')
    {
        WARNING_MSG("number contains invalid characters\n");
        return FAILURE;
    }

    if (num < INT_MIN || num > INT_MAX)
    {
        WARNING_MSG("number is too long\n");
        return FAILURE;
    }

    *output = num;
    return SUCCESS;
}

/**
 * @name parse_args
 *
 * @brief Parse tail arguments.
 *
 * @param argc
 * @param argv
 * @param opt
 * @return
 */
int parse_args (int argc, char *argv[], tail_t *opt)
{
    bool file_exist = false;
    int arg_index   = 1;

    while (arg_index < argc)
    {
        if (strcmp(argv[arg_index], "-n") == 0)
        {
            if (arg_index + 1 == argc)
            {
                WARNING_MSG("-n flag does not have a value\n");
                return FAILURE;
            }

            int number;
            if (!str_to_int(argv[arg_index + 1], &number))
                return FAILURE;

            if (*(argv[arg_index + 1]) == '-') number = ABS(number);
            else if (*(argv[arg_index + 1]) == '+') opt->type = FROM_TOP;

            opt->row_count = number;
            arg_index += 2;
        }
        else
        {
            if (file_exist)
            {
                WARNING_MSG("filename appears more than once\n");
                return FAILURE;
            }

            opt->stream     = FILE_INPUT;
            opt->filename   = argv[arg_index];

            file_exist = true;
            arg_index++;
        }
    }

    return SUCCESS;
}

/**
 * @name open_stream
 *
 * @brief Opens input stream.
 *
 * @param opt
 * @param stream
 * @return FILE *
 */
FILE * open_stream (tail_t *opt)
{
    FILE *stream = stdin;

    if (opt->stream == FILE_INPUT)
    {
        stream = fopen(opt->filename, "r");
        if (stream == NULL)
        {
            WARNING_MSG("could not open the file\n");
            return NULL;
        }
    }

    return stream;
}

/**
 * @name close_stream
 *
 * @brief Close input stream.
 *
 * @param opt
 * @param stream
 * @return void
 */
void close_stream (tail_t *opt, FILE *stream)
{
    if (opt->stream == FILE_INPUT)
        fclose(stream);
}

/**
 * @name print_from_top
 *
 * @brief Print N rows from top of the input.
 *
 * @param opt
 * @return int
 */
int print_from_top (tail_t *opt)
{
    FILE *f = open_stream(opt);
    if (f == NULL) return FAILURE;

    char row[MAX_ROW_LENGTH];
    bool overflow   = false;
    int total_count = 0;
    int chars_count = 0;

    while ((chars_count = get_line(row, MAX_ROW_LENGTH, f)) != -1)
    {
        total_count++;
        if (chars_count >= MAX_ROW_LENGTH)
        {
            if (!overflow)
            {
                WARNING_MSG("line %d is too long\n", total_count);
                overflow = true;
            }
        }

        if (total_count >= opt->row_count)
            printf("%s", row);
    }

    close_stream(opt, f);
    return SUCCESS;
}

/**
 * @name print_last_rows
 *
 * @brief Print last N rows from the input.
 *
 * @param opt
 * @return int
 */
int print_last_rows (tail_t *opt)
{
    if (!opt->row_count) return SUCCESS;

    FILE *f = open_stream(opt);
    if (f == NULL) return FAILURE;

    tail_data_t *data = data_ctor(opt->row_count);
    if (data == NULL)
    {
        close_stream(opt, f);
        return FAILURE;
    }

    char row[MAX_ROW_LENGTH];
    bool overflow   = 0;
    int total_count = 0;
    int chars_count = 0;

    while ((chars_count = get_line(row, MAX_ROW_LENGTH, f)) != -1)
    {
        total_count++;
        if (chars_count >= MAX_ROW_LENGTH)
        {
            if (!overflow)
            {
                WARNING_MSG("line %d is too long\n", total_count);
                overflow = true;
            }
        }

        data_write(data, row);
    }

    data_print(data);
    data_dtor(data);
    close_stream(opt, f);
    return SUCCESS;
}

int main (int argc, char *argv[])
{
    tail_t options = options_ctor();

    if (!parse_args(argc, argv, &options))
        return 1;

    bool print_error = false;
    if (options.type == FROM_TOP)
    {
        if (!print_from_top(&options)) print_error = true;
    }
    else
    {
        if (!print_last_rows(&options)) print_error = true;
    }

    if (print_error)
        return 1;

    return 0;
}