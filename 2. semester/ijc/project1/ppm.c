/**
 * File:        ppm.c
 * Project:     IJC-DU1, task b)
 * Author:      Evgeny Torbin
 * Date:        23.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 *
 * Description: work with PPM images (read only)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "ppm.h"

/* Max PPM image size */
#define MAX_PPM_SIZE (8000 * 8000 * 3)

/**
 * Is space
 *
 * Return 1 if character is whitespace symbol.
 *
 * @param ch
 * @return int
 */
int is_space (int ch)
{
    int whitespace = 0;

    switch (ch)
    {
        case ' ':
        case '\n':
        case '\t':
        case '\v':
        case '\f':
        case '\r':
            whitespace = 1;
            break;
    }

    return whitespace;
}

/**
 * PPM read
 *
 * Read binary data to structure from PPM file.
 *
 * @param filename
 * @return struct ppm *
 */
struct ppm * ppm_read (const char *filename)
{
    /* Check ppm format */
    const char *file_ext = strrchr(filename, '.');
    if (!file_ext || strcmp(file_ext, ".ppm") != 0)
    {
        warning_msg("%s: Wrong file format\n", __func__);
        goto func_exit;
    }

    /* Open file */
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        warning_msg("%s: Could not open the file\n", __func__);
        goto func_exit;
    }

    /* Read header information */
    char ppm_type[3];
    unsigned xsize;
    unsigned ysize;
    unsigned max_color;

    /* Parse header information */
    if (fscanf(f, "%2s %u %u %u", ppm_type, &xsize, &ysize, &max_color) != 4)
    {
        warning_msg("%s: PPM header parse error\n", __func__);
        goto fstream_close;
    }

    /* Check whitespace at the end of header */
    if (!is_space(fgetc(f)))
    {
        warning_msg("%s: PPM header parse end error\n", __func__);
        goto fstream_close;
    }

    /* Check PPM type */
    if (strcmp(ppm_type, "P6") != 0)
    {
        warning_msg("%s: Wrong PPM type\n", __func__);
        goto fstream_close;
    }

    /* Check PPM maximal color value */
    if (max_color != 255)
    {
        warning_msg("%s: Wrong PPM maximum color value\n", __func__);
        goto fstream_close;
    }

    /* Read binary data */
    unsigned long data_size = xsize * ysize * 3;

    /* Check PPM image size */
    if (data_size > MAX_PPM_SIZE)
    {
        warning_msg("%s: PPM image is too large\n", __func__);
        goto fstream_close;
    }

    /* Initialize PPM structure */
    struct ppm *image = malloc(sizeof(struct ppm) + data_size);
    if (image == NULL)
    {
        warning_msg("%s: Memory allocation error\n", __func__);
        goto fstream_close;
    }

    /* Set image sizes */
    image->xsize = xsize;
    image->ysize = ysize;

    /* Read data */
    if (fread(image->data, sizeof(unsigned char), data_size, f) != data_size)
    {
        warning_msg("%s: Binary data does not exist or shorter than image size\n", __func__);
        goto free_struct;
    }

    /* Check if data is longer than the specified size */
    if (fgetc(f) != EOF)
    {
        warning_msg("%s: Binary data is longer than image size\n", __func__);
        goto free_struct;
    }

    fclose(f);
    return image;

    /* Close handlers, free memory and exit function if error */
    free_struct:    free(image);
    fstream_close:  fclose(f);
    func_exit:      return NULL;
}

/**
 * PPM free
 *
 * Free allocated structure.
 *
 * @param p
 * @return void
 */
void ppm_free (struct ppm *p)
{
    free(p);
}