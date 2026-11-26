#include "grid.h"
#include "constants.h"
#include <stdlib.h>

void grid_init(Grid *g, const Params *p, int nr, int nz) {
    double r_max = p->R0 + 1.2 * p->a;
    double z_max = 1.2 * p->kappa * p->a;

    g->nr    = nr;
    g->nz    = nz;
    g->r_min = p->R0 - 1.2 * p->a;
    g->z_min = -z_max;
    g->dr    = (r_max - g->r_min) / (double)(nr - 1);
    g->dz    = (2.0 * z_max)      / (double)(nz - 1);
}

double grid_r(const Grid *g, int i) { return g->r_min + (double)i * g->dr; }
double grid_z(const Grid *g, int j) { return g->z_min + (double)j * g->dz; }

int field_alloc(Field *f, const Grid *g) {
    f->g = g;
    f->v = calloc((size_t)g->nr * (size_t)g->nz, sizeof(double));
    return (f->v != NULL) ? 0 : -1;
}

void field_free(Field *f) {
    free(f->v);
    f->v = NULL;
    f->g = NULL;
}

void mask_build(unsigned char *mask, const Grid *g, const Params *p) {
    double inv_a2  = 1.0 / (p->a * p->a);
    double inv_ka2 = 1.0 / (p->kappa * p->a * p->kappa * p->a);

    for (int j = 0; j < g->nz; ++j) {
        double Z = grid_z(g, j);
        for (int i = 0; i < g->nr; ++i) {
            double R  = grid_r(g, i);
            double dR = R - p->R0;
            double rho2 = dR * dR * inv_a2 + Z * Z * inv_ka2;
            mask[i + g->nr * j] = (rho2 <= 1.0) ? 1u : 0u;
        }
    }
}

double mask_volume(const unsigned char *mask, const Grid *g) {
    double vol = 0.0;
    double cell_area = g->dr * g->dz;

    for (int j = 0; j < g->nz; ++j) {
        for (int i = 0; i < g->nr; ++i) {
            if (mask[i + g->nr * j]) {
                vol += 2.0 * PHYS_PI * grid_r(g, i) * cell_area;
            }
        }
    }
    return vol;
}