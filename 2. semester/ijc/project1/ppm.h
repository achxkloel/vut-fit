/**
 * File:        ppm.h
 * Project:     IJC-DU1, task b)
 * Author:      Evgeny Torbin
 * Date:        22.03.2021
 * Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
 */

#ifndef PPM_H /* twice include guard*/
#define PPM_H

struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];
};

/* Read binary data to structure from PPM file. */
struct ppm * ppm_read(const char * filename);

/* Free allocated structure. */
void ppm_free(struct ppm *p);

#endif /* PPM_H */