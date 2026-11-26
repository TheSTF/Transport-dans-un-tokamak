#include "diagnostics.h"
#include "grid.h"
#include "io.h"
#include "params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

static void check_rel(const char *name, double got, double ref, double tol) {
    double err = fabs(got - ref) / fabs(ref);
    if (err > tol) {
        fprintf(stderr, "[FAIL] %s: got=%.6g ref=%.6g err=%.3e > %.3e\n",
                name, got, ref, err, tol);
        failures++;
    } else {
        printf("[ OK ] %s: %.6g (err=%.2e)\n", name, got, err);
    }
}

int main(void) {
    /* 1. Conversions keV <-> J */
    check_rel("keV->J->keV", joule_to_kev(kev_to_joule(12.34)), 12.34, 1e-12);

    /* 2. Diagnostics 0D : ITER et machine */
    Params iter = params_iter();
    Params mach = params_machine();

    check_rel("nG ITER",       greenwald_density(&iter), 1.194e20, 1e-3);
    check_rel("nG machine",    greenwald_density(&mach), 1.989e20, 1e-3);
    check_rel("beta_max ITER", beta_max_troyon(&iter),   3.96,     5e-3);
    check_rel("beta_max mach", beta_max_troyon(&mach),   1.40,     5e-3);
    check_rel("q95 ITER",      q95_estimate(&iter),      3.00,     1e-2);
    check_rel("q95 machine",   q95_estimate(&mach),      10.0,     1e-2);

    /* IPB98 ITER utilise kappa_a = 1.743 et P = 87 MW */
    Params iter_ipb = iter;
    iter_ipb.kappa = 1.743;
    check_rel("tau_E ITER",    tau_ipb98(&iter_ipb, 87e6), 3.68,   1e-2);
    check_rel("tau_E machine", tau_ipb98(&mach, 10e6),     0.0287, 1e-2);

    /* 3. Bornes de grille et volume du masque (65^2 puis 129^2) */
    Grid g65;
    grid_init(&g65, &mach, 65, 65);
    check_rel("R_min", grid_r(&g65, 0),  0.52,  1e-12);
    check_rel("R_max", grid_r(&g65, 64), 1.48,  1e-12);
    check_rel("Z_min", grid_z(&g65, 0), -0.816, 1e-12);
    check_rel("Z_max", grid_z(&g65, 64), 0.816, 1e-12);

    unsigned char *m65 = calloc(65 * 65, 1);
    mask_build(m65, &g65, &mach);
    check_rel("volume masque 65x65", mask_volume(m65, &g65), 5.37, 2e-2);
    free(m65);

    Grid g129;
    grid_init(&g129, &mach, 129, 129);
    unsigned char *m129 = calloc(129 * 129, 1);
    mask_build(m129, &g129, &mach);
    check_rel("volume masque 129x129", mask_volume(m129, &g129), 5.37, 1e-2);
    free(m129);

    return (failures == 0) ? 0 : 1;
}