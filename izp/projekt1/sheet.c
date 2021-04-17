/**
 *
 * Projekt 1 - Práce s textem
 *
 * @author Evgeny Torbin
 * @vut_login xtorbi00
 * @předmět IZP (akademický rok 2020/2021)
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Length of row (including '\n' and '\0')
#define MAX_ROW_LENGTH 10242

// Length of arguments (including '\0')
#define MAX_ARG_LENGTH 101

// Returns 0 if size of row with increament is less than max length of row
#define checkSizeOfRow(increament,currentSize) ((increament + currentSize < MAX_ROW_LENGTH ) ? 1 : 0)

/*
 * Get the natural number from string
 */
int getNumber(char *str)
{
    char *r;
    int number;

    number = strtol(str, &r, 10);
    if (r[0] != '\0')
        return 0;
    else if (number < 0)
        return 0;

    return number;
}

/*
 * Replace and count delimiters in row
 */
void replaceDelims(char *row, char *delims, char delim, int *delims_count)
{
    for (int i = 0; row[i] != 0; i++)
    {
        for (int j = 0; delims[j] != 0; j++)
        {
            if (row[i] == delims[j])
            {
                // Change every delimiter to the first one
                if (delims[j] != delim)
                    row[i] = delim;

                (*delims_count)++;
                break;
            }
        }
    }
}

/*
 * Get actual delimiters positions and one additional position in row
 */
void getDelimsPos(char *row, char delim, int *delims_pos)
{
    int delims_pos_index = 1;

    // Extra position before first column
    delims_pos[0] = 0;

    for (int i = 0; row[i] != 0; i++)
    {
        if (row[i] == delim)
            delims_pos[delims_pos_index++] = i;
    }
}

/*
 * Print empty row
 */
void printEmptyRow(char ch, int count)
{
    for (int i = 0; i < count; i++)
        printf("%c", ch);

    printf("\n");
}

/*
 * Check if number of actual row is equal to the given number
 */
bool insertRow(int before, int actual)
{
    if (before == actual)
        return true;

    return false;
}

/*
 * Count rows to append
 */
void appendRow(int actual, int *count)
{
    if (actual == 1)
        (*count)++;
}

/*
 * Delete rows
 */
bool deleteRow(int remove, int actual)
{
    if (remove == actual)
        return false;

    return true;
}

/*
 * Delete rows
 */
bool deleteRows(int from, int to, int actual)
{
    if (from <= to && actual >= from && actual <= to)
        return false;

    return true;
}

/*
 * Insert column
 */
void insertColumn(int before, char *row, char delim, int *delims_count)
{
    int columns_count = *delims_count + 1;

    if (before <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        if(!checkSizeOfRow(1, r_len))
            return;
        int delim_index = before - 1;
        int move_from = delims_pos[delim_index];

        // Move characters from the end of row
        for (int j = r_len; j >= move_from; j--)
            row[j + 1] = row[j];

        row[move_from] = delim;
        (*delims_count)++;
    }
}

/*
 * Append column
 */
void appendColumn(char *row, char delim, int *delims_count)
{
    int r_len = strlen(row);
    if(!checkSizeOfRow(1, r_len))
        return;
    // Move '\n', '\0' and paste delimiter
    row[r_len + 1] = row[r_len];
    row[r_len] = row[r_len - 1];
    row[r_len - 1] = delim;

    (*delims_count)++;
}

/*
 * Delete column
 */
void deleteColumn(int remove, char *row, char delim, int *delims_count)
{
    int columns_count = *delims_count + 1;

    if (remove <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int delim_index = remove - 1;

        // Check if last column or not
        if (remove == columns_count)
        {
            // Write '\n' and '\0' instead of last column
            row[delims_pos[delim_index]] = row[r_len - 1];
            row[delims_pos[delim_index] + 1] = row[r_len];
        }
        else
        {
            int c_len = delims_pos[delim_index + 1] - delims_pos[delim_index];

            // Check if first column and increment length of column
            if (remove == 1)
                c_len++;

            // Move characters from start of column by length
            for (int i = delims_pos[delim_index]; i <= r_len - c_len; i++)
                row[i] = row[i + c_len];
        }

        (*delims_count)--;
    }
}

/*
 * Delete columns
 */
void deleteColumns(int from, int to, char *row, char delim, int *delims_count)
{
    int columns_count = *delims_count + 1;

    if (to <= columns_count && from <= to)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int delim_start = from - 1;
        int delim_end = to;

        // Check if last column or not
        if (to == columns_count)
        {
            row[delims_pos[delim_start]] = row[r_len - 1];
            row[delims_pos[delim_start] + 1] = row[r_len];
        }
        else
        {
            int c_len = delims_pos[delim_end] - delims_pos[delim_start];

            // Check if first column and increment length of column
            if (from == 1)
                c_len++;

            // Move characters from start of columns by length
            for (int i = delims_pos[delim_start]; i <= r_len - c_len; i++)
                row[i] = row[i + c_len];
        }

        (*delims_count) -= to - from + 1;
    }
}

/*
 * Set the value to a column
 */
void columnSet(int column, char *str, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;
    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int s_len = strlen(str);
        int c_len;

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        c_len = column_end - column_start;
        if(!checkSizeOfRow(c_len, r_len))
            return;
        int move_count;

        if (s_len > c_len)
        {
            // Add space to column
            move_count = s_len - c_len;
            for (int j = r_len + 1; j >= column_end; j--)
                row[j + move_count] = row[j];
        }
        else if (c_len > s_len)
        {
            // Delete space from column
            move_count = c_len - s_len;
            for (int i = column_start; i <= r_len - move_count; i++)
                row[i] = row[i + move_count];
        }

        // Write given string
        for (int i = column_start, j = 0; j < s_len; i++, j++)
        {
            row[i] = str[j];
        }
    }
}

/*
 * Convert letters in column to lowercase
 */

void toLowerCase(int column, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        // Difference between codes of small and big letter
        int diff = 'a' - 'A';

        // Change letters
        for (int i = column_start; i < column_end; i++)
            if (row[i] >= 'A' && row[i] <= 'Z')
                row[i] += diff;
    }
}

/*
 * Convert letters in column to uppercase
 */
void toUpperCase(int column, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        // Difference between codes of small and big letter
        int diff = 'a' - 'A';

        // Change letters
        for (int i = column_start; i < column_end; i++)
            if (row[i] >= 'a' && row[i] <= 'z')
                row[i] -= diff;
    }
}

/*
 * Round a number
 */
void roundFunc(int column, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int c_len;

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        c_len = column_end - column_start;
        char col_value[c_len + 1];

        for (int i = column_start, j = 0; i < column_end; i++, j++) {
            col_value[j] = row[i];
        }

        // Add null terminating byte
        col_value[c_len] = '\0';

        char *r;
        double num;
        num = strtod(col_value, &r);

        if (r[0] == '\0')
        {
            if (num > 0.0)
                sprintf(col_value, "%d", (int) (num + 0.5));
            else
                sprintf(col_value, "%d", (int) (num - 0.5));
        }

        columnSet(column, col_value, row, delim, delims_count);
    }
}

/*
 * Convert number with decimal point to integer
 */
void intFunc(int column, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int c_len;

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        c_len = column_end - column_start;
        char col_value[c_len + 1];

        for (int i = column_start, j = 0; i < column_end; i++, j++) {
            col_value[j] = row[i];
        }

        // Add null terminating byte
        col_value[c_len] = '\0';

        char *r;
        double num;
        num = strtod(col_value, &r);

        if (r[0] == '\0')
        {
            sprintf(col_value, "%d", (int) num);
        }

        columnSet(column, col_value, row, delim, delims_count);
    }
}

/*
 * Copy value from one column to another
 */
void copy(int from, int to, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (from != to && from <= columns_count && to <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int c_len;

        // Delimiter start position
        int delim_start = from - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (from == 1)
            column_start = 0;

        if (from == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        c_len = column_end - column_start;
        char str_to_copy[c_len + 1];

        // Write value of column
        for (int i = column_start, j = 0; i < column_end; i++, j++) {
            str_to_copy[j] = row[i];
        }
        // Add null terminating byte
        str_to_copy[c_len] = '\0';

        // Paste
        columnSet(to, str_to_copy, row, delim, delims_count);
    }
}

/*
 * Swap values from two columns
 */
void swap(int first, int second, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (first != second && first <= columns_count && second <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);

        int delim_first_start = first - 1;
        int delim_second_start = second - 1;

        int column_first_start = delims_pos[delim_first_start] + 1;
        int column_second_start = delims_pos[delim_second_start] + 1;

        int column_first_end;
        int column_second_end;

        if (first == 1)
            column_first_start = 0;

        if (first == columns_count)
        {
            column_first_end = r_len - 1;
        }
        else
        {
            column_first_end = delims_pos[delim_first_start + 1];
        }

        if (second == 1)
            column_second_start = 0;

        if (second == columns_count)
        {
            column_second_end = r_len - 1;
        }
        else
        {
            column_second_end = delims_pos[delim_second_start + 1];
        }

        char column_first[column_first_end - column_first_start + 1];
        char column_second[column_second_end - column_second_start + 1];

        for (int i = column_first_start, j = 0; i < column_first_end; i++, j++) {
            column_first[j] = row[i];
        }

        for (int i = column_second_start, j = 0; i < column_second_end; i++, j++) {
            column_second[j] = row[i];
        }

        column_first[column_first_end - column_first_start] = '\0';
        column_second[column_second_end - column_second_start] = '\0';

        columnSet(first, column_second, row, delim, delims_count);
        columnSet(second, column_first, row, delim, delims_count);
    }
}

/*
 * Move one column before another
 */
void move(int first, int second, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (first != second && first <= columns_count && second <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int c_len;

        // Delimiter start position
        int delim_start = first - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (first == 1)
            column_start = 0;

        if (first == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        c_len = column_end - column_start;
        char str_to_move[c_len + 1];

        for (int i = column_start, j = 0; i < column_end; i++, j++) {
            str_to_move[j] = row[i];
        }

        // Add null terminating byte
        str_to_move[c_len] = '\0';

        if (second > first)
            second--;

        // Move
        deleteColumn(first, row, delim, &delims_count);
        insertColumn(second, row, delim, &delims_count);
        columnSet(second, str_to_move, row, delim, delims_count);
    }
}

/*
 * Select rows by numbers or special symbols
 */
bool rows(char *from, char *to, int actual) {
    int n = getNumber(from);
    int m = getNumber(to);

    if (n)
    {
        if (m)
        {
            if (n <= actual && actual <= m)
                return true;
        }
        else if (actual >= n)
        {
            return true;
        }
    }
    else
    {
        if (m)
        {
            if (m >= actual)
                return true;
        }
        else
        {
            // Check if row is last
            int ch = fgetc(stdin);
            int last_row = 0;

            if (ch == EOF)
                last_row = 1;

            ungetc(ch, stdin);

            if (last_row)
                return true;
            else
                return false;
        }
    }

    return false;
}

/*
 * Check if row begins with the given string
 */
bool beginsWith(int column, char *str, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int s_len = strlen(str);
        int c_len;

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        // Check if begins with the given string
        c_len = column_end - column_start;

        if (s_len >= c_len)
        {
            for (int i = column_start, j = 0; i < column_end; i++, j++)
            {
                if (row[i] != str[j]) {
                    return false;
                }
            }
            return true;
        }
        else
        {
            for (int i = column_start, j = 0; j < s_len; i++, j++)
            {
                if (row[i] != str[j]) {
                    return false;
                }
            }
            return true;
        }

        return false;
    }

    return false;
}

/*
 * Check if row contains the given string
 */
bool contains(int column, char *str, char *row, char delim, int delims_count)
{
    int columns_count = delims_count + 1;

    if (column <= columns_count)
    {
        int delims_pos[columns_count];
        getDelimsPos(row, delim, delims_pos);

        int r_len = strlen(row);
        int c_len;

        // Delimiter start position
        int delim_start = column - 1;

        // Column start, end positions
        int column_start = delims_pos[delim_start] + 1;
        int column_end;

        if (column == 1)
            column_start = 0;

        if (column == columns_count)
            column_end = r_len - 1;
        else
            column_end = delims_pos[delim_start + 1];

        c_len = column_end - column_start;
        char str_in_col[c_len + 1];

        for (int i = column_start, j = 0; i < column_end; i++, j++) {
            str_in_col[j] = row[i];
        }

        // Add null terminating byte
        str_in_col[c_len] = '\0';

        // If the given string is substring of column value
        if (strstr(str_in_col, str))
            return true;

        return false;
    }

    return false;
}

/*
 * Sort arguments
 */
int sortArguments(int argc, char *argv[], int *commands_count, char *commands[])
{
    int modify_commands = 0;
    int select_commands = 0;
    int data_commands = 0;

    *commands_count = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strlen(argv[i]) > MAX_ARG_LENGTH - 1)
        {
            fprintf(stderr, "Error: argument %d is too big\n", i);
            return 1;
        }

        // Table modification commands
        if (strcmp(argv[i], "irow") == 0 ||
            strcmp(argv[i], "arow") == 0 ||
            strcmp(argv[i], "drow") == 0 ||
            strcmp(argv[i], "drows") == 0 ||
            strcmp(argv[i], "icol") == 0 ||
            strcmp(argv[i], "acol") == 0 ||
            strcmp(argv[i], "dcol") == 0 ||
            strcmp(argv[i], "dcols") == 0)
        {
            if (data_commands || select_commands)
            {
                fprintf(stderr, "Error: using table modification with other commands\n");
                return 1;
            }

            // Modification commands with 2 parameters
            if (strcmp(argv[i], "drows") == 0 ||
                strcmp(argv[i], "dcols") == 0)
            {
                // Check if parameters of command exist
                if (i+2 < argc)
                {
                    // Check if parameters are numbers
                    if (getNumber(argv[i+1]) && getNumber(argv[i+2]))
                    {
                        // Write to array of commands
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i+1];
                        commands[(*commands_count)++] = argv[i+2];
                        modify_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s %s\n", argv[i], argv[i+1], argv[i+2]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameters of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
                // Modification commands with 1 parameter
            else if (strcmp(argv[i], "irow") == 0 ||
                     strcmp(argv[i], "drow") == 0 ||
                     strcmp(argv[i], "icol") == 0 ||
                     strcmp(argv[i], "dcol") == 0)
            {
                if (i+1 < argc)
                {
                    if (getNumber(argv[i+1]))
                    {
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i+1];
                        modify_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s\n", argv[i], argv[i+1]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameter of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
                // Modification commands with 0 parameters
            else
            {
                commands[(*commands_count)++] = argv[i];
                modify_commands++;
            }
        }

            // Data processing commands
        else if (strcmp(argv[i], "cset") == 0 ||
                 strcmp(argv[i], "tolower") == 0 ||
                 strcmp(argv[i], "toupper") == 0 ||
                 strcmp(argv[i], "round") == 0 ||
                 strcmp(argv[i], "int") == 0 ||
                 strcmp(argv[i], "copy") == 0 ||
                 strcmp(argv[i], "swap") == 0 ||
                 strcmp(argv[i], "move") == 0)
        {
            if (modify_commands)
            {
                fprintf(stderr, "Error: using data processing with table modification\n");
                return 1;
            }
            else if (data_commands)
            {
                fprintf(stderr, "Error: using more than 1 data processing command\n");
                return 1;
            }

            // Data processing command with 2 parameters (number and string)
            if (strcmp(argv[i], "cset") == 0)
            {
                if (i + 2 < argc)
                {
                    if (getNumber(argv[i + 1]))
                    {
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i + 1];
                        commands[(*commands_count)++] = argv[i + 2];
                        data_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s %s\n", argv[i], argv[i + 1], argv[i + 2]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameters of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
                // Data processing commands with 2 parameters (numbers)
            else if (strcmp(argv[i], "copy") == 0 ||
                     strcmp(argv[i], "swap") == 0 ||
                     strcmp(argv[i], "move") == 0)
            {
                if (i + 2 < argc)
                {
                    if (getNumber(argv[i + 1]) && getNumber(argv[i + 2]))
                    {
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i + 1];
                        commands[(*commands_count)++] = argv[i + 2];
                        data_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s %s\n", argv[i], argv[i + 1], argv[i + 2]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameters of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
                // Data processing commands with 1 parameter
            else
            {
                if (i + 1 < argc)
                {
                    if (getNumber(argv[i + 1]))
                    {
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i + 1];
                        data_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s\n", argv[i], argv[i + 1]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameter of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
        }

            // Table select commands
        else if (strcmp(argv[i], "rows") == 0 ||
                 strcmp(argv[i], "beginswith") == 0 ||
                 strcmp(argv[i], "contains") == 0)
        {
            if (modify_commands)
            {
                fprintf(stderr, "Error: using selection with table modification\n");
                return 1;
            }
            else if (select_commands)
            {
                fprintf(stderr, "Error: using more than 1 selection command\n");
                return 1;
            }
            else if (data_commands)
            {
                fprintf(stderr, "Error: selection command must be before data processing command\n");
                return 1;
            }

            // Select command with 2 parameters (numbers or '-')
            if (strcmp(argv[i], "rows") == 0)
            {
                if (i + 2 < argc)
                {
                    if ((getNumber(argv[i+1]) || strcmp(argv[i+1], "-") == 0) &&
                        (getNumber(argv[i+2]) || strcmp(argv[i+2], "-") == 0))
                    {
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i + 1];
                        commands[(*commands_count)++] = argv[i + 2];
                        select_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s %s\n", argv[i], argv[i + 1], argv[i + 2]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameters of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
                // Select commands with 2 parameters (number and string)
            else
            {
                if (i + 2 < argc)
                {
                    if (getNumber(argv[i + 1]))
                    {
                        commands[(*commands_count)++] = argv[i];
                        commands[(*commands_count)++] = argv[i + 1];
                        commands[(*commands_count)++] = argv[i + 2];
                        select_commands++;
                    }
                    else
                    {
                        fprintf(stderr, "Error: bad command %s %s %s\n", argv[i], argv[i + 1], argv[i + 2]);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: parameters of '%s' is out of range\n", argv[i]);
                    return 1;
                }
            }
        }
    }

    // Check using selection with data processing and on the contrary
    if (select_commands && !data_commands)
    {
        fprintf(stderr, "Error: using selection without data processing\n");
        return 1;
    }

    // Check if commands exist
    if (!modify_commands && !data_commands && !select_commands)
    {
        fprintf(stderr, "Error: invalid commands\n");
        return 1;
    }

    return 0;
}

/*
 * Main
 */
int main(int argc, char *argv[])
{
    /*
     * Find delimiter
     */

    // Array of delimiters
    char *delims = " ";

    if (argc > 2)
    {
        if (strcmp(argv[1], "-d") == 0)
        {
            delims = argv[2];
        }
    }
    else
    {
        fprintf(stderr, "Error: delimiter not found\n");
        return 1;
    }

    // First delimiter
    char delim = delims[0];

    /*
     * Sort arguments
     */

    // Array of commands
    char *commands[MAX_ARG_LENGTH];
    int commands_count;

    if (sortArguments(argc, argv, &commands_count, commands))
        return 1;

    /*
     * Read from stdin by one row
     */

    char row[MAX_ROW_LENGTH];

    // Row number
    int row_number = 1;

    // Amount of delimiters in one row
    int delims_count;

    // Amount of delimiters in first row
    int first_delims_count;

    // Values if row must be printed, inserted or selected for data processing
    bool print_row;
    bool insert_row;
    bool select_row;

    // Count of rows to append in the end of table
    int rows_append_count = 0;

	while (fgets(row, MAX_ROW_LENGTH, stdin) != NULL)
	{
        // Replace and count delimiters in row
        delims_count = 0;
        replaceDelims(row, delims, delim, &delims_count);

        // Check amount of delimiters in row (except first)
        if (row_number == 1)
        {
            first_delims_count = delims_count;
        }
        else
        {
            // If row has incorrect amount of delimiters - skip iteration
            if (first_delims_count != delims_count)
            {
                printf("%s", row);
                row_number++;
                continue;
            }
        }

        /*
         * Command processing
         */

        print_row = true;
        insert_row = false;
        select_row = true;

        for (int i = 0; i < commands_count; i++)
        {
            if (strcmp(commands[i], "irow") == 0)
            {
                if (insert_row == false)
                {
                    int r = getNumber(commands[i+1]);
                    insert_row = insertRow(r, row_number);
                }
            }
            else if (strcmp(commands[i], "arow") == 0)
            {
                appendRow(row_number, &rows_append_count);
            }
            else if (strcmp(commands[i], "drow") == 0)
            {
                int r = getNumber(commands[i+1]);
                print_row = deleteRow(r, row_number);
            }
            else if (strcmp(commands[i], "drows") == 0)
            {
                int n = getNumber(commands[i+1]);
                int m = getNumber(commands[i+2]);
                print_row = deleteRows(n, m, row_number);
            }
            else if (strcmp(commands[i], "icol") == 0)
            {
                int c = getNumber(commands[i+1]);
                insertColumn(c, row, delim, &delims_count);
            }
            else if (strcmp(commands[i], "acol") == 0)
            {
                appendColumn(row, delim, &delims_count);
            }
            else if (strcmp(commands[i], "dcol") == 0)
            {
                int c = getNumber(commands[i+1]);
                deleteColumn(c, row, delim, &delims_count);
            }
            else if (strcmp(commands[i], "dcols") == 0)
            {
                int n = getNumber(commands[i+1]);
                int m = getNumber(commands[i+2]);
                deleteColumns(n, m, row, delim, &delims_count);
            }
            else if (strcmp(commands[i], "rows") == 0)
            {
                char *n = commands[i+1];
                char *m = commands[i+2];
                select_row = rows(n, m, row_number);
            }
            else if (strcmp(commands[i], "beginswith") == 0)
            {
                int c = getNumber(commands[i+1]);
                char *str = commands[i+2];
                select_row = beginsWith(c, str, row, delim, delims_count);
            }
            else if (strcmp(commands[i], "contains") == 0)
            {
                int c = getNumber(commands[i+1]);
                char *str = commands[i+2];
                select_row = contains(c, str, row, delim, delims_count);
            }

            if (select_row == true)
            {
                if (strcmp(commands[i], "cset") == 0)
                {
                    int c = getNumber(commands[i + 1]);
                    char *str = commands[i + 2];
                    columnSet(c, str, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "tolower") == 0)
                {
                    int c = getNumber(commands[i+1]);
                    toLowerCase(c, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "toupper") == 0)
                {
                    int c = getNumber(commands[i+1]);
                    toUpperCase(c, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "round") == 0)
                {
                    int c = getNumber(commands[i+1]);
                    roundFunc(c, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "int") == 0)
                {
                    int c = getNumber(commands[i+1]);
                    intFunc(c, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "copy") == 0)
                {
                    int n = getNumber(commands[i+1]);
                    int m = getNumber(commands[i+2]);
                    copy(n, m, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "swap") == 0)
                {
                    int n = getNumber(commands[i+1]);
                    int m = getNumber(commands[i+2]);
                    swap(n, m, row, delim, delims_count);
                }
                else if (strcmp(commands[i], "move") == 0)
                {
                    int n = getNumber(commands[i+1]);
                    int m = getNumber(commands[i+2]);
                    move(n, m, row, delim, delims_count);
                }
            }
        }

        // Insert empty row
        if (insert_row == true)
            printEmptyRow(delim, delims_count);

        // Print row
        if (print_row == true)
            printf("%s", row);

        // Increment row number
        row_number++;
	}

    // Append rows
    if (rows_append_count)
    {
        for (int i = 0; i < rows_append_count; i++)
        {
            printEmptyRow(delim, delims_count);
        }
    }

	return 0;
}
