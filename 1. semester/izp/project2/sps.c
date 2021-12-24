/**
 *
 * Projekt 2 - Práce s datovými strukturami
 *
 * @author Evgeny Torbin
 * @vut_login xtorbi00
 * @předmět IZP (akademický rok 2020/2021)
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_LENGTH 1001
#define MAX_COMMAND_AMOUNT 1000
#define MAX_TMP_AMOUNT 10

// List od error codes
enum errcodes
{
    ERR_FOPEN = 1,
    ERR_FCLOSE,
    ERR_MEMORY,
    ERR_ARGS,
    ERR_MANY_COMMANDS,
    ERR_COMMAND_SIZE,
    ERR_TABLE_EMPTY,
    ERR_INVALID_COMMAND
};

// List of selection types (for Select structure)
enum select_types
{
    SELECT_CELL = 1,
    SELECT_ROW,
    SELECT_COLUMN,
    SELECT_WINDOW,
    SELECT_TABLE
};

/*
 * Cell structure
 */
typedef struct Cell
{
    char *data;
} cell_t;

/*
 * Row structure
 */
typedef struct Row
{
    cell_t *cell;
} row_t;

/*
 * Table structure
 */
typedef struct Table
{
    int rows_count;
    int cols_count;
    row_t *row;
} table_t;

/*
 * Selection structure
 */
typedef struct Select
{
    int row_first;
    int col_first;
    int row_second;
    int col_second;

    int select_type;
} select_t;

/*
 * Command structure
 */
typedef struct Command
{
    int count;
    char *list[MAX_COMMAND_AMOUNT];
} command_t;

/*
 * Temporary variables structure
 */
typedef struct Temp
{
    char *tmp[MAX_TMP_AMOUNT];

    int row_first;
    int col_first;
    int row_second;
    int col_second;

    int select_type;
} temp_t;

/*
 * Print error exceptions
 */
void errorException (int err_num)
{
    // Error prefix
    char *prefix = "[ERROR]";

    // List of error messages
    static const char *err_list[] =
    {
        "Could not open the file",
        "Could not close the file",
        "Not enough memory",
        "Not enough arguments",
        "Too many commands",
        "Size of command is too big",
        "Table is empty",
        "Invalid command"
    };

    // Print error with prefix, error code and message
    fprintf(stderr, "%s[%d] %s\n", prefix, err_num, err_list[err_num - 1]);
}

/*
 * Read table from file
 */
int readFromFile(FILE **fp, char *delims, table_t *table)
{
    // Work with file
    int c;
    char ch;
    int cells_max = 0;

    // Find rows and cols count
    while((c = fgetc(*fp)) != EOF)
    {
        ch = (char) c;

        if (strchr(delims, ch) != NULL)
        {
            cells_max++;
        }
        else if (ch == '\n')
        {
            table->rows_count++;
            cells_max++;

            if (cells_max > table->cols_count)
                table->cols_count = cells_max;

            cells_max = 0;
        }
    }

    // Allocate table
    if (!table->rows_count && !table->cols_count)
        return ERR_TABLE_EMPTY;

    table->row = malloc(table->rows_count * sizeof(row_t));
    if (table->row == NULL)
        return ERR_MEMORY;

    for (int i = 0; i < table->rows_count; i++)
    {
        table->row[i].cell = malloc(table->cols_count * sizeof(cell_t));
        if (table->row[i].cell == NULL)
            return ERR_MEMORY;
    }

    // Write data
    rewind(*fp);
    int r_count = 0;
    int c_count = 0;
    int length = 0;
    while ((c = fgetc(*fp)) != EOF)
    {
        ch = (char) c;

        if (strchr(delims, ch) == NULL && ch != '\n')
        {
            if (!length)
                table->row[r_count].cell[c_count].data = malloc(2 * sizeof(char));
            else
                table->row[r_count].cell[c_count].data = realloc(table->row[r_count].cell[c_count].data, (length + 2) * sizeof(char));

            if (table->row[r_count].cell[c_count].data == NULL)
                return ERR_MEMORY;

            table->row[r_count].cell[c_count].data[length] = ch;
            length++;
        }
        else
        {
            if (length == 0)
            {
                table->row[r_count].cell[c_count].data = malloc(sizeof(char));

                if (table->row[r_count].cell[c_count].data == NULL)
                    return ERR_MEMORY;
            }

            table->row[r_count].cell[c_count].data[length] = '\0';

            if (strchr(delims, ch) != NULL)
            {
                length = 0;
                c_count++;
            }
            else
            {
                if (c_count + 1 < table->cols_count)
                {
                    for (int i = c_count + 1; i < table->cols_count; i++)
                    {
                        table->row[r_count].cell[i].data = malloc(sizeof(char));
                        if (table->row[r_count].cell[c_count].data == NULL)
                            return ERR_MEMORY;

                        table->row[r_count].cell[i].data[0] = '\0';
                    }
                }
                length = 0;
                c_count = 0;
                r_count++;
            }
        }
    }

    return 0;
}

/*
 * Write table to file
 */
int writeToFile(char *filename, char delim, table_t *table)
{
    // Open file
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
        return ERR_FOPEN;

    // Write to file
    for (int i = 0; i < table->rows_count; i++)
    {
        for (int j = 0; j < table->cols_count; j++)
        {
            fprintf(fp, "%s", table->row[i].cell[j].data);
            if (j + 1 != table->cols_count)
                fprintf(fp, "%c", delim);
        }
        fprintf(fp, "\n");
    }

    // Close file
    if (fclose(fp) == EOF)
        return ERR_FCLOSE;

    return 0;
}

/*
 * Print the table
 */
void printTable(char delim, table_t *table)
{
    for (int i = 0; i < table->rows_count; i++)
    {
        for (int j = 0; j < table->cols_count; j++)
        {
            printf("%s", table->row[i].cell[j].data);
            if (j + 1 != table->cols_count)
                printf("%c", delim);
        }
        printf("\n");
    }
}

/*
 * Find commands
 */
int findCommands(char *sequence, command_t *command)
{
    char *token = strtok(sequence, ";");

    while (token != NULL)
    {
        if (command->count == MAX_COMMAND_AMOUNT)
            return ERR_MANY_COMMANDS;

        if (strlen(token) >= MAX_COMMAND_LENGTH)
            return ERR_COMMAND_SIZE;

        command->list[command->count] = token;
        token = strtok(NULL, ";");
        command->count++;
    }

    return 0;
}

/*
 * Insert row before selected
 */
int insertRow(select_t *select, table_t *table)
{
    if(select->select_type != 1)
        return 0;

    table->row = realloc(table->row, (table->rows_count + 1) * sizeof(row_t));
    if (table->row == NULL)
        return ERR_MEMORY;

    table->row[table->rows_count].cell = malloc(table->cols_count * sizeof(cell_t));
    for (int i = 0; i < table->cols_count; i++)
    {
        table->row[table->rows_count].cell[i].data = malloc(sizeof(char));
        if (table->row[table->rows_count].cell[i].data == NULL)
            return ERR_MEMORY;

        table->row[table->rows_count].cell[i].data[0] = '\0';
    }

    row_t tmp;
    for (int i = table->rows_count; i > select->row_first - 1; i--)
    {
        tmp = table->row[i];
        table->row[i] = table->row[i - 1];
        table->row[i - 1] = tmp;
    }

    table->rows_count++;
    return 0;
}

/*
 * Append row after selected
 */
int appendRow(select_t *select, table_t *table)
{
    if(select->select_type != 1)
        return 0;

    table->row = realloc(table->row, (table->rows_count + 1) * sizeof(row_t));
    if (table->row == NULL)
        return ERR_MEMORY;

    table->row[table->rows_count].cell = malloc(table->cols_count * sizeof(cell_t));
    for (int i = 0; i < table->cols_count; i++)
    {
        table->row[table->rows_count].cell[i].data = malloc(sizeof(char));
        if (table->row[table->rows_count].cell[i].data == NULL)
            return ERR_MEMORY;

        table->row[table->rows_count].cell[i].data[0] = '\0';
    }

    row_t tmp;
    for (int i = table->rows_count; i > select->row_first; i--)
    {
        tmp = table->row[i];
        table->row[i] = table->row[i - 1];
        table->row[i - 1] = tmp;
    }

    table->rows_count++;
    return 0;
}

/*
 * Delete selected row
 */
int deleteRow(select_t *select, table_t *table)
{
    if(select->select_type != 1)
        return 0;

    row_t tmp;
    for (int i = select->row_first - 1; i < table->rows_count - 1; i++)
    {
        tmp = table->row[i];
        table->row[i] = table->row[i + 1];
        table->row[i + 1] = tmp;
    }

    for (int i = 0; i < table->cols_count; i++)
    {
        free(table->row[table->rows_count - 1].cell[i].data);
        table->row[table->rows_count - 1].cell[i].data = NULL;
    }
    free(table->row[table->rows_count - 1].cell);
    table->row[table->rows_count - 1].cell = NULL;

    if ((table->rows_count - 1) == 0)
    {
        free(table->row);
        table->row= NULL;
    }
    else
        table->row = realloc(table->row, (table->rows_count - 1) * sizeof(row_t));

    table->rows_count--;
    if (table->rows_count == 0)
        table->cols_count = 0;
    return 0;
}

/*
 * Insert column before selected
 */
int insertColumn(select_t *select, table_t *table)
{
    if(select->select_type != 1)
        return 0;

    cell_t tmp;
    for (int i = 0; i < table->rows_count; i++)
    {
        table->row[i].cell = realloc(table->row[i].cell, (table->cols_count + 1) * sizeof(cell_t));
        if (table->row[i].cell == NULL)
            return ERR_MEMORY;

        table->row[i].cell[table->cols_count].data = malloc(sizeof(char));
        if (table->row[i].cell[table->cols_count].data == NULL)
            return ERR_MEMORY;

        table->row[i].cell[table->cols_count].data[0] = '\0';
        for (int j = table->cols_count; j > select->col_first - 1; j--)
        {
            tmp = table->row[i].cell[j];
            table->row[i].cell[j] = table->row[i].cell[j - 1];
            table->row[i].cell[j - 1] = tmp;
        }
    }

    table->cols_count++;
    return 0;
}

/*
 * Append column before selected
 */
int appendColumn(select_t *select, table_t *table)
{
    if(select->select_type != 1)
        return 0;

    cell_t tmp;
    for (int i = 0; i < table->rows_count; i++)
    {
        table->row[i].cell = realloc(table->row[i].cell, (table->cols_count + 1) * sizeof(cell_t));
        if (table->row[i].cell == NULL)
            return ERR_MEMORY;

        table->row[i].cell[table->cols_count].data = malloc(sizeof(char));
        if (table->row[i].cell[table->cols_count].data == NULL)
            return ERR_MEMORY;

        table->row[i].cell[table->cols_count].data[0] = '\0';
        for (int j = table->cols_count; j > select->col_first; j--)
        {
            tmp = table->row[i].cell[j];
            table->row[i].cell[j] = table->row[i].cell[j - 1];
            table->row[i].cell[j - 1] = tmp;
        }
    }

    table->cols_count++;
    return 0;
}

/*
 * Delete selected column
 */
int deleteColumn(select_t *select, table_t *table)
{
    if(select->select_type != 1)
        return 0;

    for (int i = 0; i < table->rows_count; i++)
    {
        cell_t tmp;
        for (int j = select->col_first - 1; j < table->cols_count - 1; j++)
        {
            tmp = table->row[i].cell[j];
            table->row[i].cell[j] = table->row[i].cell[j + 1];
            table->row[i].cell[j + 1] = tmp;
        }

        free(table->row[i].cell[table->cols_count - 1].data);
        table->row[i].cell[table->cols_count - 1].data = NULL;

        if ((table->cols_count - 1) == 0)
        {
            free(table->row[i].cell);
            table->row[i].cell = NULL;
        }
        else
            table->row[i].cell = realloc(table->row[i].cell, (table->cols_count - 1) * sizeof(cell_t));
    }

    table->cols_count--;
    if (table->cols_count == 0)
    {
        free(table->row);
        table->row = NULL;
        table->rows_count = 0;
    }
    return 0;
}

/*
 * Set selected cells to string
 */
int setCell(select_t *select, table_t *table, char *str)
{
    char **data;

    switch (select->select_type) {
        case SELECT_CELL:
            data = &table->row[select->row_first - 1].cell[select->col_first - 1].data;
            *data = realloc(*data, (strlen(str) + 1) * sizeof(char));
            if (*data == NULL)
                return ERR_MEMORY;
            strcpy(*data, str);
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                *data = realloc(*data, (strlen(str) + 1) * sizeof(char));
                if (*data == NULL)
                    return ERR_MEMORY;
                strcpy(*data, str);
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                *data = realloc(*data, (strlen(str) + 1) * sizeof(char));
                if (*data == NULL)
                    return ERR_MEMORY;
                strcpy(*data, str);
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    *data = realloc(*data, (strlen(str) + 1) * sizeof(char));
                    if (*data == NULL)
                        return ERR_MEMORY;
                    strcpy(*data, str);
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    *data = realloc(*data, (strlen(str) + 1) * sizeof(char));
                    if (*data == NULL)
                        return ERR_MEMORY;
                    strcpy(*data, str);
                }
            }
            break;
    }

    return 0;
}

/*
 * Clear selected cells
 */
int clearCell(select_t *select, table_t *table)
{
    int data_length;
    char **data;
    switch (select->select_type) {
        case SELECT_CELL:
            data = &table->row[select->row_first - 1].cell[select->col_first - 1].data;
            data_length = strlen(*data);
            if (data_length > 0)
            {
                *data = realloc(*data, sizeof(char));
                *data[0] = '\0';
            }
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                data_length = strlen(*data);
                if (data_length > 0)
                {
                    *data = realloc(*data, sizeof(char));
                    *data[0] = '\0';
                }
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                data_length = strlen(*data);
                if (data_length > 0)
                {
                    *data = realloc(*data, sizeof(char));
                    *data[0] = '\0';
                }
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    data_length = strlen(*data);
                    if (data_length > 0)
                    {
                        *data = realloc(*data, sizeof(char));
                        *data[0] = '\0';
                    }
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    data_length = strlen(*data);
                    if (data_length > 0)
                    {
                        *data = realloc(*data, sizeof(char));
                        *data[0] = '\0';
                    }
                }
            }
            break;
    }

    return 0;
}

/*
 * Swap selected cells
 */
int swapCell(select_t *select, table_t *table, int row, int column)
{
    cell_t tmp;

    switch (select->select_type) {
        case SELECT_CELL:
            tmp = table->row[row - 1].cell[column - 1];
            table->row[row - 1].cell[column - 1] = table->row[select->row_first - 1].cell[select->col_first - 1];
            table->row[select->row_first - 1].cell[select->col_first - 1] = tmp;
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                tmp = table->row[row - 1].cell[column - 1];
                table->row[row - 1].cell[column - 1] = table->row[select->row_first - 1].cell[i];
                table->row[select->row_first - 1].cell[i] = tmp;
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                tmp = table->row[row - 1].cell[column - 1];
                table->row[row - 1].cell[column - 1] = table->row[i].cell[select->col_first - 1];
                table->row[i].cell[select->col_first - 1] = tmp;
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    tmp = table->row[row - 1].cell[column - 1];
                    table->row[row - 1].cell[column - 1] = table->row[i].cell[j];
                    table->row[i].cell[j] = tmp;
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    tmp = table->row[row - 1].cell[column - 1];
                    table->row[row - 1].cell[column - 1] = table->row[i].cell[j];
                    table->row[i].cell[j] = tmp;
                }
            }
            break;
    }

    return 0;
}

/*
 * Get a sum of selected cells
 */
int sumCell(select_t *select, table_t *table, int row, int column)
{
    char **data;
    char **selected = &table->row[row - 1].cell[column - 1].data;
    double cell;
    char *r;
    double sum = 0;
    int count = 0;
    char *sum_str;
    int size_str;

    switch (select->select_type) {
        case SELECT_CELL:
            data = &table->row[select->row_first - 1].cell[select->col_first - 1].data;
            cell = strtod(*data, &r);
            if (r[0] == '\0' && *data[0] != '\0')
            {
                sum += cell;
                count++;
            }

            if (!count)
                return 0;

            size_str = snprintf(NULL, 0, "%g", sum) + 1;
            sum_str = malloc(size_str * sizeof(char));
            if (sum_str == NULL)
                return ERR_MEMORY;
            sprintf(sum_str, "%g", sum);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, sum_str);
            free(sum_str);
            sum_str = NULL;
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    sum += cell;
                    count++;
                }
            }

            if (!count)
                return 0;

            size_str = snprintf(NULL, 0, "%g", sum) + 1;
            sum_str = malloc(size_str * sizeof(char));
            if (sum_str == NULL)
                return ERR_MEMORY;
            sprintf(sum_str, "%g", sum);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, sum_str);
            free(sum_str);
            sum_str = NULL;
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    sum += cell;
                    count++;
                }
            }

            if (!count)
                return 0;

            size_str = snprintf(NULL, 0, "%g", sum) + 1;
            sum_str = malloc(size_str * sizeof(char));
            if (sum_str == NULL)
                return ERR_MEMORY;
            sprintf(sum_str, "%g", sum);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, sum_str);
            free(sum_str);
            sum_str = NULL;
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        sum += cell;
                        count++;
                    }
                }
            }

            if (!count)
                return 0;

            size_str = snprintf(NULL, 0, "%g", sum) + 1;
            sum_str = malloc(size_str * sizeof(char));
            if (sum_str == NULL)
                return ERR_MEMORY;
            sprintf(sum_str, "%g", sum);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, sum_str);
            free(sum_str);
            sum_str = NULL;
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        sum += cell;
                        count++;
                    }
                }
            }

            if (!count)
                return 0;

            size_str = snprintf(NULL, 0, "%g", sum) + 1;
            sum_str = malloc(size_str * sizeof(char));
            if (sum_str == NULL)
                return ERR_MEMORY;
            sprintf(sum_str, "%g", sum);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, sum_str);
            free(sum_str);
            sum_str = NULL;
            break;
    }

    return 0;
}

/*
 * Get an average of selected cells
 */
int averageCell(select_t *select, table_t *table, int row, int column)
{
    char **data;
    char **selected = &table->row[row - 1].cell[column - 1].data;
    double cell;
    char *r;
    double sum = 0;
    double avg = 0;
    int count = 0;
    char *avg_str;
    int size_str;

    switch (select->select_type) {
        case SELECT_CELL:
            data = &table->row[select->row_first - 1].cell[select->col_first - 1].data;
            cell = strtod(*data, &r);
            if (r[0] == '\0' && *data[0] != '\0')
            {
                sum += cell;
                count++;
            }

            if (!count)
                return 0;

            avg = sum/count;
            size_str = snprintf(NULL, 0, "%g", avg) + 1;
            avg_str = malloc(size_str * sizeof(char));
            if (avg_str == NULL)
                return ERR_MEMORY;
            sprintf(avg_str, "%g", avg);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, avg_str);
            free(avg_str);
            avg_str = NULL;
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    sum += cell;
                    count++;
                }
            }

            if (!count)
                return 0;

            avg = sum/count;
            size_str = snprintf(NULL, 0, "%g", avg) + 1;
            avg_str = malloc(size_str * sizeof(char));
            if (avg_str == NULL)
                return ERR_MEMORY;
            sprintf(avg_str, "%g", avg);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, avg_str);
            free(avg_str);
            avg_str = NULL;
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    sum += cell;
                    count++;
                }
            }

            if (!count)
                return 0;

            avg = sum/count;
            size_str = snprintf(NULL, 0, "%g", avg) + 1;
            avg_str = malloc(size_str * sizeof(char));
            if (avg_str == NULL)
                return ERR_MEMORY;
            sprintf(avg_str, "%g", avg);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, avg_str);
            free(avg_str);
            avg_str = NULL;
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        sum += cell;
                        count++;
                    }
                }
            }

            if (!count)
                return 0;

            avg = sum/count;
            size_str = snprintf(NULL, 0, "%g", avg) + 1;
            avg_str = malloc(size_str * sizeof(char));
            if (avg_str == NULL)
                return ERR_MEMORY;
            sprintf(avg_str, "%g", avg);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, avg_str);
            free(avg_str);
            avg_str = NULL;
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        sum += cell;
                        count++;
                    }
                }
            }

            if (!count)
                return 0;

            avg = sum/count;
            size_str = snprintf(NULL, 0, "%g", avg) + 1;
            avg_str = malloc(size_str * sizeof(char));
            if (avg_str == NULL)
                return ERR_MEMORY;
            sprintf(avg_str, "%g", avg);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, avg_str);
            free(avg_str);
            avg_str = NULL;
            break;
    }

    return 0;
}

/*
 * Count empty cells
 */
int countCell(select_t *select, table_t *table, int row, int column)
{
    char **data;
    char **selected = &table->row[row - 1].cell[column - 1].data;
    int count = 0;
    char *count_str;
    int size_str;

    switch (select->select_type) {
        case SELECT_CELL:
            data = &table->row[select->row_first - 1].cell[select->col_first - 1].data;
            if (strlen(*data) > 0)
                count++;

            size_str = snprintf(NULL, 0, "%d", count) + 1;
            count_str = malloc(size_str * sizeof(char));
            if (count_str == NULL)
                return ERR_MEMORY;
            sprintf(count_str, "%d", count);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, count_str);
            free(count_str);
            count_str = NULL;
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                if (strlen(*data) > 0)
                    count++;

                size_str = snprintf(NULL, 0, "%d", count) + 1;
                count_str = malloc(size_str * sizeof(char));
                if (count_str == NULL)
                    return ERR_MEMORY;
                sprintf(count_str, "%d", count);

                *selected = realloc(*selected, size_str * sizeof(char));
                if (*selected == NULL)
                    return ERR_MEMORY;
                strcpy(*selected, count_str);
                free(count_str);
                count_str = NULL;
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                if (strlen(*data) > 0)
                    count++;

                size_str = snprintf(NULL, 0, "%d", count) + 1;
                count_str = malloc(size_str * sizeof(char));
                if (count_str == NULL)
                    return ERR_MEMORY;
                sprintf(count_str, "%d", count);

                *selected = realloc(*selected, size_str * sizeof(char));
                if (*selected == NULL)
                    return ERR_MEMORY;
                strcpy(*selected, count_str);
                free(count_str);
                count_str = NULL;
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    if (strlen(*data) > 0)
                        count++;

                    size_str = snprintf(NULL, 0, "%d", count) + 1;
                    count_str = malloc(size_str * sizeof(char));
                    if (count_str == NULL)
                        return ERR_MEMORY;
                    sprintf(count_str, "%d", count);

                    *selected = realloc(*selected, size_str * sizeof(char));
                    if (*selected == NULL)
                        return ERR_MEMORY;
                    strcpy(*selected, count_str);
                    free(count_str);
                    count_str = NULL;
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    if (strlen(*data) > 0)
                        count++;

                    size_str = snprintf(NULL, 0, "%d", count) + 1;
                    count_str = malloc(size_str * sizeof(char));
                    if (count_str == NULL)
                        return ERR_MEMORY;
                    sprintf(count_str, "%d", count);

                    *selected = realloc(*selected, size_str * sizeof(char));
                    if (*selected == NULL)
                        return ERR_MEMORY;
                    strcpy(*selected, count_str);
                    free(count_str);
                    count_str = NULL;
                }
            }
            break;
    }

    return 0;
}

/*
 * Save length of cell
 */
int lengthCell(select_t *select, table_t *table, int row, int column)
{
    char **data;
    char **selected = &table->row[row - 1].cell[column - 1].data;
    int length;
    char *length_str;
    int size_str;

    switch (select->select_type) {
        case SELECT_CELL:
            data = &table->row[select->row_first - 1].cell[select->col_first - 1].data;
            length = strlen(*data);

            size_str = snprintf(NULL, 0, "%d", length) + 1;
            length_str = malloc(size_str * sizeof(char));
            if (length_str == NULL)
                return ERR_MEMORY;
            sprintf(length_str, "%d", length);

            *selected = realloc(*selected, size_str * sizeof(char));
            if (*selected == NULL)
                return ERR_MEMORY;
            strcpy(*selected, length_str);
            free(length_str);
            length_str = NULL;
            break;
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                length = strlen(*data);

                size_str = snprintf(NULL, 0, "%d", length) + 1;
                length_str = malloc(size_str * sizeof(char));
                if (length_str == NULL)
                    return ERR_MEMORY;
                sprintf(length_str, "%d", length);

                *selected = realloc(*selected, size_str * sizeof(char));
                if (*selected == NULL)
                    return ERR_MEMORY;
                strcpy(*selected, length_str);
                free(length_str);
                length_str = NULL;
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                length = strlen(*data);

                size_str = snprintf(NULL, 0, "%d", length) + 1;
                length_str = malloc(size_str * sizeof(char));
                if (length_str == NULL)
                    return ERR_MEMORY;
                sprintf(length_str, "%d", length);

                *selected = realloc(*selected, size_str * sizeof(char));
                if (*selected == NULL)
                    return ERR_MEMORY;
                strcpy(*selected, length_str);
                free(length_str);
                length_str = NULL;
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    length = strlen(*data);

                    size_str = snprintf(NULL, 0, "%d", length) + 1;
                    length_str = malloc(size_str * sizeof(char));
                    if (length_str == NULL)
                        return ERR_MEMORY;
                    sprintf(length_str, "%d", length);

                    *selected = realloc(*selected, size_str * sizeof(char));
                    if (*selected == NULL)
                        return ERR_MEMORY;
                    strcpy(*selected, length_str);
                    free(length_str);
                    length_str = NULL;
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    length = strlen(*data);

                    size_str = snprintf(NULL, 0, "%d", length) + 1;
                    length_str = malloc(size_str * sizeof(char));
                    if (length_str == NULL)
                        return ERR_MEMORY;
                    sprintf(length_str, "%d", length);

                    *selected = realloc(*selected, size_str * sizeof(char));
                    if (*selected == NULL)
                        return ERR_MEMORY;
                    strcpy(*selected, length_str);
                    free(length_str);
                    length_str = NULL;
                }
            }
            break;
    }

    return 0;
}

/*
 * Expand table
 */
int expandTable (int rows_add, int cols_add, table_t *table, select_t *select)
{
    int expand_error;

    for (int i = 0; i < rows_add; i++)
    {
        expand_error = appendRow(select, table);
        if (expand_error) return expand_error;
    }

    for (int i = 0; i < cols_add; i++)
    {
        expand_error = appendColumn(select, table);
        if (expand_error) return expand_error;
    }

    return 0;
}

/*
 * Check if one string begins with another
 */
int beginsWith(char *first, char *second)
{
    return strncmp(first, second, strlen(first)) == 0;
}

/*
 * Check if one string is equal to another
 */
int isEqual(char *first, char *second)
{
    return strcmp(first, second) == 0;
}

/*
 * Select cell
 */
int selectCell(int row, int column, table_t *table, select_t *select)
{
    int error = 0;

    if (row > 0 && column > 0)
    {
        if (row > table->rows_count || column > table->cols_count)
        {
            int row_diff = 0;
            int col_diff = 0;

            select->select_type = 1;
            if (row > table->rows_count)
                row_diff = row - table->rows_count;
            if (column > table->cols_count)
                col_diff = column - table->cols_count;

            select->row_first = table->rows_count;
            select->col_first = table->cols_count;

            error = expandTable(row_diff, col_diff, table, select);
            if (error) return error;
        }

        select->select_type = 1;
        select->row_first = row;
        select->col_first = column;
    }
    else
        return ERR_INVALID_COMMAND;

    return 0;
}

/*
 * Select row
 */
int selectRow(int row, table_t *table, select_t *select)
{
    int error = 0;

    if (row > 0)
    {
        if (row > table->rows_count)
        {
            int row_diff;
            select->select_type = 1;
            row_diff = row - table->rows_count;

            select->row_first = table->rows_count;

            error = expandTable(row_diff, 0, table, select);
            if (error) return error;
        }

        select->select_type = 2;
        select->row_first = row;
    }
    else
        return ERR_INVALID_COMMAND;

    return 0;
}

/*
 * Select column
 */
int selectColumn(int column, table_t *table, select_t *select)
{
    int error = 0;

    if (column > 0)
    {
        if (column > table->cols_count)
        {
            int col_diff;
            select->select_type = 1;
            col_diff = column - table->cols_count;

            select->col_first = table->cols_count;

            error = expandTable(0, col_diff, table, select);
            if (error) return error;
        }

        select->select_type = 3;
        select->col_first = column;
    }
    else
        return ERR_INVALID_COMMAND;

    return 0;
}

/*
 * Select window
 */
int selectWindow(int row_from, int column_from, int row_to, int column_to, table_t *table, select_t *select)
{
    int error = 0;

    if (row_from > 0 && column_from > 0 && row_to >= row_from && column_to >= column_from)
    {
        if (row_to > table->rows_count || column_to > table->cols_count)
        {
            int row_diff = 0;
            int col_diff = 0;

            select->select_type = 1;
            if (row_to > table->rows_count)
                row_diff = row_to - table->rows_count;
            if (column_to > table->cols_count)
                col_diff = column_to - table->cols_count;

            select->row_first = table->rows_count;
            select->col_first = table->cols_count;

            error = expandTable(row_diff, col_diff, table, select);
            if (error) return error;
        }

        select->select_type = 4;
        select->row_first = row_from;
        select->col_first = column_from;
        select->row_second = row_to;
        select->col_second = column_to;
    }
    else
        return ERR_INVALID_COMMAND;

    return 0;
}

/*
 * Select minimum
 */
int selectMin(table_t *table, select_t *select)
{
    char **data;
    char *r;
    double cell;
    double min;
    int is_exist = 0;

    switch (select->select_type) {
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    if (is_exist == 0)
                    {
                        select->select_type = 1;
                        select->col_first = i + 1;
                        min = cell;
                        is_exist++;
                    }

                    if (cell < min)
                    {
                        select->select_type = 1;
                        select->col_first = i + 1;
                        min = cell;
                    }
                }
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    if (is_exist == 0)
                    {
                        select->select_type = 1;
                        select->row_first = i + 1;
                        min = cell;
                        is_exist++;
                    }

                    if (cell < min)
                    {
                        select->select_type = 1;
                        select->row_first = i + 1;
                        min = cell;
                    }
                }
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        if (is_exist == 0)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = j + 1;
                            min = cell;
                            is_exist++;
                        }

                        if (cell < min)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = j + 1;
                            min = cell;
                        }
                    }
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        if (is_exist == 0)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = j + 1;
                            min = cell;
                            is_exist++;
                        }

                        if (cell < min)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = j + 1;
                            min = cell;
                        }
                    }
                }
            }
            break;
    }

    return 0;
}

/*
 * Select maximum
 */
int selectMax(table_t *table, select_t *select)
{
    char **data;
    char *r;
    double cell;
    double max;
    int is_exist = 0;

    switch (select->select_type) {
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    if (is_exist == 0)
                    {
                        select->select_type = 1;
                        select->col_first = i + 1;
                        max = cell;
                        is_exist++;
                    }

                    if (cell > max)
                    {
                        select->select_type = 1;
                        select->col_first = i + 1;
                        max = cell;
                    }
                }
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                cell = strtod(*data, &r);
                if (r[0] == '\0' && *data[0] != '\0')
                {
                    if (is_exist == 0)
                    {
                        select->select_type = 1;
                        select->row_first = i + 1;
                        max = cell;
                        is_exist++;
                    }

                    if (cell > max)
                    {
                        select->select_type = 1;
                        select->row_first = i + 1;
                        max = cell;
                    }
                }
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        if (is_exist == 0)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = i + 1;
                            max = cell;
                            is_exist++;
                        }

                        if (cell > max)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = i + 1;
                            max = cell;
                        }
                    }
                }
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    cell = strtod(*data, &r);
                    if (r[0] == '\0' && *data[0] != '\0')
                    {
                        if (is_exist == 0)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = i + 1;
                            max = cell;
                            is_exist++;
                        }

                        if (cell > max)
                        {
                            select->select_type = 1;
                            select->row_first = i + 1;
                            select->col_first = i + 1;
                            max = cell;
                        }
                    }
                }
            }
            break;
    }

    return 0;
}

/*
 * Select first cell, which contains string
 */
int selectContains(table_t *table, select_t *select, char *str)
{
    char **data;
    int found = 0;

    switch (select->select_type) {
        case SELECT_ROW:
            for (int i = 0; i < table->cols_count; i++)
            {
                data = &table->row[select->row_first - 1].cell[i].data;
                if (strstr(*data, str) != NULL)
                {
                    select->select_type = 1;
                    select->col_first = i + 1;
                    break;
                }
            }
            break;
        case SELECT_COLUMN:
            for (int i = 0; i < table->rows_count; i++)
            {
                data = &table->row[i].cell[select->col_first - 1].data;
                if (strstr(*data, str) != NULL)
                {
                    select->select_type = 1;
                    select->row_first = i + 1;
                    break;
                }
            }
            break;
        case SELECT_WINDOW:
            for (int i = select->row_first - 1; i < select->row_second; i++)
            {
                for (int j = select->col_first - 1; j < select->col_second; j++)
                {
                    data = &table->row[i].cell[j].data;
                    if (strstr(*data, str) != NULL)
                    {
                        select->select_type = 1;
                        select->row_first = i + 1;
                        select->col_first = j + 1;
                        found++;
                        break;
                    }
                }
                if (found)
                    break;
            }
            break;
        case SELECT_TABLE:
            for (int i = 0; i < table->rows_count; i++)
            {
                for (int j = 0; j < table->cols_count; j++)
                {
                    data = &table->row[i].cell[j].data;
                    if (strstr(*data, str) != NULL)
                    {
                        select->select_type = 1;
                        select->row_first = i + 1;
                        select->col_first = j + 1;
                        found++;
                        break;
                    }
                }
                if (found)
                    break;
            }
            break;
    }

    return 0;
}

/*
 * Change selection
 */
int cellSelection(char *command, select_t *select, table_t *table)
{
    int r;
    int c;
    int r1;
    int c1;
    char row_dash = 'u';
    char col_dash = 'u';
    char underscore;
    int commas_count = 0;
    int select_error;

    command++;
    command[strlen(command) - 1] = '\0';

    if (beginsWith("find", command))
    {
        char *opt;
        char *token = strtok(command, " ");
        token = strtok(NULL, "\0");
        if (token != NULL)
            opt = token;
        else
            return ERR_INVALID_COMMAND;

        select_error = selectContains(table, select, opt);
        if (select_error) return select_error;
    }
    else
    {
        for (int i = 0; command[i] != 0; i++)
            if (command[i] == ',')
                commas_count++;

        if (strchr(command, ' ') != NULL)
            return ERR_INVALID_COMMAND;

        if (sscanf(command, "%d,%d", &r, &c) == 2 && commas_count == 1)
        {
            select_error = selectCell(r, c, table, select);
            if (select_error) return select_error;
        }
        else if (sscanf(command, "%d,%c", &r, &underscore) == 2 && commas_count == 1)
        {
            if (underscore != '_')
                return ERR_INVALID_COMMAND;

            select_error = selectRow(r, table, select);
            if (select_error) return select_error;
        }
        else if (sscanf(command, "%c,%d", &underscore, &c) == 2 && commas_count == 1)
        {
            if (row_dash != '_')
                return ERR_INVALID_COMMAND;

            select_error = selectColumn(c, table, select);
            if (select_error) return select_error;
        }
        else if ((sscanf(command, "%d,%d,%d,%d", &r, &c, &r1, &c1) == 4 ||
                 sscanf(command, "%d,%d,%c,%c", &r, &c, &row_dash, &col_dash) == 4 ||
                 sscanf(command, "%d,%d,%d,%c", &r, &c, &r1, &col_dash) == 4 ||
                 sscanf(command, "%d,%d,%c,%d", &r, &c, &row_dash, &c1) == 4) && commas_count == 3)
        {
            if (row_dash == '-' && col_dash == '-')
            {
                r1 = table->rows_count;
                c1 = table->cols_count;
            }
            else if (row_dash == '-' && col_dash != '-')
                r1 = table->rows_count;
            else if (row_dash != '-' && col_dash == '-')
                c1 = table->cols_count;

            select_error = selectWindow(r, c, r1, c1, table, select);
            if (select_error) return select_error;
        }
        else if (isEqual(command, "_,_"))
        {
            select->select_type = 5;
        }
        else if (isEqual(command, "min"))
        {
            select_error = selectMin(table, select);
            if (select_error) return select_error;
        }
        else if (isEqual(command, "max"))
        {
            select_error = selectMax(table, select);
            if (select_error) return select_error;
        }
        else
            return ERR_INVALID_COMMAND;
    }

    return 0;
}

/*
 * Define a temporary variable
 */
int tmpDefine(int value, select_t *select, table_t *table, temp_t *temp)
{
    if (value < 0 || value > 9)
        return ERR_INVALID_COMMAND;

    if (select->select_type != SELECT_CELL)
        return ERR_INVALID_COMMAND;

    char *data = table->row[select->row_first - 1].cell[select->col_first - 1].data;

    if (temp->tmp[value] == NULL)
        temp->tmp[value] = malloc(strlen(data) + 1);
    else
        temp->tmp[value] = realloc(temp->tmp[value], strlen(data) + 1);

    if (temp->tmp[value] == NULL)
        return ERR_MEMORY;

    strcpy(temp->tmp[value], data);

    return 0;
}

/*
 * Use a temporary variable
 */
int tmpUse(int value, select_t *select, table_t *table, temp_t *temp)
{
    int use_error;

    if (value < 0 || value > 9)
        return ERR_INVALID_COMMAND;

    if (temp->tmp[value] == NULL)
        return ERR_INVALID_COMMAND;

    use_error = setCell(select, table, temp->tmp[value]);
    if (use_error) return use_error;

    return 0;
}

/*
 * Increment a temporary variable
 */
int tmpIncrement(int value, temp_t *temp)
{
    if (value < 0 || value > 9)
        return ERR_INVALID_COMMAND;

    if (temp->tmp[value] == NULL)
        return ERR_INVALID_COMMAND;

    char *r;
    double inc;
    int size_str;

    inc = strtod(temp->tmp[value], &r);
    if (r[0] == '\0' && temp->tmp[value][0] != '\0')
        inc++;
    else
        inc = 1.0;

    size_str = snprintf(NULL, 0, "%g", inc) + 1;
    temp->tmp[value] = realloc(temp->tmp[value], size_str * sizeof(char) + 1);
    if (temp->tmp[value] == NULL)
        return ERR_MEMORY;
    sprintf(temp->tmp[value], "%g", inc);

    return 0;
}

/*
 * Set a temporary variable
 */
int tmpSet(select_t *select, temp_t *temp)
{
    temp->select_type = select->select_type;

    switch (select->select_type)
    {
        case SELECT_CELL:
            temp->row_first = select->row_first;
            temp->col_first = select->col_first;
            break;
        case SELECT_ROW:
            temp->row_first = select->row_first;
            break;
        case SELECT_COLUMN:
            temp->col_first = select->col_first;
            break;
        case SELECT_WINDOW:
            temp->row_first = select->row_first;
            temp->row_second = select->row_second;
            temp->row_second = select->row_second;
            temp->col_second = select->col_second;
            break;
    }

    return 0;
}

/*
 * Select from temporary variable
 */
int tmpSelect(select_t *select, temp_t *temp)
{
    select->select_type = temp->select_type;

    switch (temp->select_type)
    {
        case SELECT_CELL:
            select->row_first = temp->row_first;
            select->col_first = temp->col_first;
            break;
        case SELECT_ROW:
            select->row_first = temp->row_first;
            break;
        case SELECT_COLUMN:
            select->col_first = temp->col_first;
            break;
        case SELECT_WINDOW:
            select->row_first = temp->row_first;
            select->row_second = temp->row_second;
            select->row_second = temp->row_second;
            select->col_second = temp->col_second;
            break;
    }

    return 0;
}

/*
 * Work with temporary variables
 */
int tmpSelection(char *command, select_t *select, table_t *table, temp_t *temp)
{
    int tmp_error;
    char *opt;
    char *token;
    int value;

    if (isEqual("[set]", command))
    {
        tmp_error = tmpSet(select, temp);
        if (tmp_error) return tmp_error;
    }
    else if (isEqual("[_]", command))
    {
        tmp_error = tmpSelect(select, temp);
        if (tmp_error) return tmp_error;
    }
    else
    {
        token = strtok(command, " ");
        token = strtok(NULL, "\0");
        if (token != NULL)
            opt = token;
        else
            return ERR_INVALID_COMMAND;
        if (sscanf(opt, "_%d", &value) != 1)
            return ERR_INVALID_COMMAND;

        if (beginsWith("def", command))
        {
            tmp_error = tmpDefine(value, select, table, temp);
            if (tmp_error) return tmp_error;
        }
        if (beginsWith("use", command))
        {
            tmp_error = tmpUse(value, select, table, temp);
            if (tmp_error) return tmp_error;
        }
        else if (beginsWith("inc", command))
        {
            tmp_error = tmpIncrement(value, temp);
            if (tmp_error) return tmp_error;
        }
    }

    return 0;
}

/*
 * Modify table
 */
int tableModification(char *command, select_t *select, table_t *table)
{
    int modify_error;

    if (isEqual("irow", command))
    {
        modify_error = insertRow(select, table);
        if (modify_error) return modify_error;
    }
    else if (isEqual("arow", command))
    {
        modify_error = appendRow(select, table);
        if (modify_error) return modify_error;
    }
    else if (isEqual("drow", command))
    {
        modify_error = deleteRow(select, table);
        if (modify_error) return modify_error;
    }
    else if (isEqual("icol", command))
    {
        modify_error = insertColumn(select, table);
        if (modify_error) return modify_error;
    }
    else if (isEqual("acol", command))
    {
        modify_error = appendColumn(select, table);
        if (modify_error) return modify_error;
    }
    else if (isEqual("dcol", command))
    {
        modify_error = deleteColumn(select, table);
        if (modify_error) return modify_error;
    }

    return 0;
}

/*
 * Table data processing
 */
int tableDataProcessing (char *command, select_t *select, table_t *table)
{
    int data_error;

    if (isEqual("clear", command))
    {
        data_error = clearCell(select, table);
        if (data_error) return data_error;
    }
    else
    {
        char *opt;
        char *token;
        int row;
        int column;
        int commas_count = 0;

        token = strtok(command, " ");
        token = strtok(NULL, "\0");
        if (token != NULL)
            opt = token;
        else
            return ERR_INVALID_COMMAND;

        if(beginsWith("set", command))
        {
            data_error = setCell(select, table, opt);
            if (data_error) return data_error;
        }
        else
        {
            opt++;
            opt[strlen(opt) - 1] = '\0';

            for (int i = 0; opt[i] != 0; i++)
                if (opt[i] == ',')
                    commas_count++;

            if (sscanf(opt, "%d,%d", &row, &column) != 2 || commas_count != 1 || strchr(opt, ' ') != NULL)
                return ERR_INVALID_COMMAND;

            if (row <= 0 || column <= 0 || row > table->rows_count || column > table->cols_count)
                return ERR_INVALID_COMMAND;

            if (beginsWith("swap", command))
            {
                data_error = swapCell(select, table, row, column);
                if (data_error) return data_error;
            }
            else if (beginsWith("sum", command))
            {
                data_error = sumCell(select, table, row, column);
                if (data_error) return data_error;
            }
            else if (beginsWith("avg", command))
            {
                data_error = averageCell(select, table, row, column);
                if (data_error) return data_error;
            }
            else if (beginsWith("count", command))
            {
                data_error = countCell(select, table, row, column);
                if (data_error) return data_error;
            }
            else if (beginsWith("len", command))
            {
                data_error = lengthCell(select, table, row, column);
                if (data_error) return data_error;
            }
        }
    }

    return 0;
}

/*
 * Command processing
 */
int commandProcessing(command_t *command, select_t *select, table_t *table, temp_t *temp)
{
    int command_error;

    for (int i = 0; i < command->count; i++)
    {
        if (!table->rows_count && !table->cols_count)
            return ERR_TABLE_EMPTY;

        // Change cells
        if (command->list[i][0] == '[' && command->list[i][strlen(command->list[i]) - 1] == ']' &&
            !isEqual("[set]", command->list[i]) &&
            !isEqual("[_]", command->list[i]))
        {
            command_error = cellSelection(command->list[i], select, table);
            if (command_error) return command_error;
        }
        // Change temporary variables
        else if (beginsWith("def", command->list[i]) ||
                beginsWith("use", command->list[i]) ||
                beginsWith("inc", command->list[i]) ||
                isEqual("[set]", command->list[i]) ||
                isEqual("[_]", command->list[i]))
        {
            command_error = tmpSelection(command->list[i], select, table, temp);
            if (command_error) return command_error;
        }
        // Modify table
        else if (isEqual("irow", command->list[i]) ||
                isEqual("arow", command->list[i]) ||
                isEqual("drow", command->list[i]) ||
                isEqual("icol", command->list[i]) ||
                isEqual("acol", command->list[i]) ||
                isEqual("dcol", command->list[i]))
        {
            command_error = tableModification(command->list[i], select, table);
            if (command_error) return command_error;
        }
        // Edit table data
        else if (beginsWith("set", command->list[i]) ||
                isEqual("clear", command->list[i]) ||
                beginsWith("swap", command->list[i]) ||
                beginsWith("sum", command->list[i]) ||
                beginsWith("avg", command->list[i]) ||
                beginsWith("count", command->list[i]) ||
                beginsWith("len", command->list[i]))
        {
            command_error = tableDataProcessing(command->list[i], select, table);
            if (command_error) return command_error;
        }
        // Goto commands
        else if (beginsWith("goto", command->list[i]) ||
                beginsWith("iszero", command->list[i]) ||
                beginsWith("sub", command->list[i]))
        {
            // TODO
        }
        else
            return ERR_INVALID_COMMAND;
    }

    return 0;
}

/*
 * Free table
 */
void freeTable(table_t *table)
{
    if (!table->rows_count && !table->cols_count)
        return;

    for (int i = 0; i < table->rows_count; i++)
    {
        for (int j = 0; j < table->cols_count; j++)
        {
            free(table->row[i].cell[j].data);
            table->row[i].cell[j].data = NULL;
        }
        free(table->row[i].cell);
        table->row[i].cell = NULL;
    }
    free(table->row);
    table->row = NULL;
}

/*
 * Free temporary variables
 */
void freeTemp (temp_t *temp)
{
    for (int i = 0; i < MAX_TMP_AMOUNT; i++)
    {
        if (temp->tmp[i] != NULL)
            free(temp->tmp[i]);
    }
}

/*
 * Initialize program
 */
int init (int argc, char *argv[]) {
    int init_error;

    /* Init table structure */
    table_t table;
    table.rows_count = 0;
    table.cols_count = 0;

    /* Init select structure */
    select_t select;
    select.select_type = SELECT_CELL;
    select.row_first = 1;
    select.col_first = 1;

    /* Init command structure */
    command_t command;
    command.count = 0;

    /* Init temporary variables structure */
    temp_t temp;
    temp.select_type = SELECT_CELL;
    temp.row_first = 1;
    temp.col_first = 1;
    for (int i = 0; i < MAX_TMP_AMOUNT; i++)
        temp.tmp[i] = NULL;

    /* Check amount of arguments */
    if (argc < 3)
        return ERR_ARGS;

    /* Find delimiters */
    char *delims = " ";
    if (argc > 2) {
        if (isEqual("-d", argv[1]))
            delims = argv[2];
    }
    char delim = delims[0];

    /* File commands */
    init_error = findCommands(argv[argc - 2], &command);
    if (init_error) return init_error;

    /* Open the file */
    FILE *fp = fopen(argv[argc - 1], "r");
    if (fp == NULL)
        return ERR_FOPEN;

    /* Read table from file */
    init_error = readFromFile(&fp, delims, &table);
    if (init_error)
    {
        if (fclose(fp) == EOF)
            return ERR_FCLOSE;

        return init_error;
    }

    /* Close the file */
    if (fclose(fp) == EOF)
        return ERR_FCLOSE;

    /* Command processing */
    init_error = commandProcessing(&command, &select, &table, &temp);
    if (init_error)
    {
        freeTable(&table);
        return init_error;
    }

    /* Write table to file */
    /*init_error = writeToFile(argv[argc - 1], delim, &table);
    if (init_error) return init_error;*/

     /* Print table */
    printTable(delim, &table);

    /* Free table */
    freeTable(&table);

    /* Fre temporary variables */
    freeTemp(&temp);

    return 0;
}

int main(int argc, char *argv[])
{
    int e = init(argc, argv);
    if (e)
    {
        errorException(e);
        return e;
    }

    return 0;
}
