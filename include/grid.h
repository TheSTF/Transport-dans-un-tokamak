#ifndef TOKAMAK_GRID_H
#define TOKAMAK_GRID_H

#include "params.h"

typedef struct {
    int nr;
    int nz;
    double r_min;
    double z_min;
    double dr;
    double dz;
} Grid;

typedef struct {
    const Grid *g;
    double *v; /* stockage linearise : v[i + nr * j], i rapide */
} Field;

void   grid_init(Grid *g, const Params *p, int nr, int nz);
double grid_r(const Grid *g, int i);
double grid_z(const Grid *g, int j);

int    field_alloc(Field *f, const Grid *g);
void   field_free(Field *f);

void   mask_build(unsigned char *mask, const Grid *g, const Params *p);
double mask_volume(const unsigned char *mask, const Grid *g);

#endif /* TOKAMAK_GRID_H */