/* src/io.c */
#include "io.h"
#include <stdio.h>

#ifndef GIT_HASH
#define GIT_HASH "unknown"
#endif

int io_write_csv(const char *path, const Field *f) {
    if (!path || !f || !f->g || !f->v) {
        return -1;
    }
    FILE *fp = fopen(path, "w");
    if (!fp) {
        return -1;
    }
    fprintf(fp, "# git=%s nr=%d nz=%d\n", GIT_HASH, f->g->nr, f->g->nz);
    fprintf(fp, "R,Z,value\n");

    for (int j = 0; j < f->g->nz; ++j) {
        double Z = grid_z(f->g, j);
        for (int i = 0; i < f->g->nr; ++i) {
            double R = grid_r(f->g, i);
            fprintf(fp, "%.8g,%.8g,%.10g\n", R, Z, f->v[i + f->g->nr * j]);
        }
    }
    return (fclose(fp) == 0) ? 0 : -1;
}