#include "diagnostics.h"
#include "grid.h"
#include "io.h"
#include "params.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Params p = params_machine();
    params_print(&p);

    printf("V      = %.3f m^3\n", plasma_volume(&p));
    printf("n_G    = %.3e m^-3 (f_GW = %.2f)\n",
           greenwald_density(&p), p.n / greenwald_density(&p));
    printf("q95    = %.2f\n", q95_estimate(&p));
    printf("beta_m = %.2f %%\n", beta_max_troyon(&p));
    printf("tau_E  = %.4f s (IPB98 a %.1f MW)\n",
           tau_ipb98(&p, p.P_nbi_nom), p.P_nbi_nom * 1e-6);
    printf("P_LH   = %.2f MW\n", p_lh_martin(&p) * 1e-6);

    Grid g;
    grid_init(&g, &p, 65, 65);

    unsigned char *mask = calloc((size_t)g.nr * (size_t)g.nz, sizeof(unsigned char));
    Field f;
    if (!mask || field_alloc(&f, &g) != 0) {
        free(mask);
        fprintf(stderr, "Erreur d'allocation memoire\n");
        return 1;
    }

    mask_build(mask, &g, &p);
    printf("V_mask = %.3f m^3 (65x65)\n", mask_volume(mask, &g));

    for (int k = 0; k < g.nr * g.nz; ++k) {
        f.v[k] = (double)mask[k];
    }

    int rc = io_write_csv("mask.csv", &f);
    field_free(&f);
    free(mask);

    if (rc != 0) {
        fprintf(stderr, "Erreur d'ecriture de mask.csv\n");
        return 1;
    }
    return 0;
}