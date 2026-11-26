#include "diagnostics.h"
#include "constants.h"
#include <math.h>

double kev_to_joule(double T_keV) {
    return T_keV * PHYS_KEV_TO_J;
}

double joule_to_kev(double T_J) {
    return T_J / PHYS_KEV_TO_J;
}

double plasma_volume(const Params *p) {
    return 2.0 * PHYS_PI * PHYS_PI * p->R0 * p->a * p->a * p->kappa;
}

double greenwald_density(const Params *p) {
    double Ip_MA = p->Ip * 1.0e-6;
    double nG_20 = Ip_MA / (PHYS_PI * p->a * p->a);
    return nG_20 * 1.0e20;
}

double beta_max_troyon(const Params *p) {
    double Ip_MA = p->Ip * 1.0e-6;
    return 2.8 * Ip_MA / (p->a * p->B0);
}

double q95_estimate(const Params *p) {
    double Ip_MA = p->Ip * 1.0e-6;
    double eps   = p->a / p->R0;
    double d     = p->delta;
    double k     = p->kappa;

    double q_cyl = 5.0 * p->a * p->a * p->B0 / (p->R0 * Ip_MA);
    double shape = (1.0 + k * k * (1.0 + 2.0 * d * d - 1.2 * d * d * d)) / 2.0;
    double tor   = (1.17 - 0.65 * eps) / ((1.0 - eps * eps) * (1.0 - eps * eps));
    return q_cyl * shape * tor;
}

double tau_ipb98(const Params *p, double P_loss_W) {
    double Ip_MA = p->Ip * 1.0e-6;
    double n19   = p->n * 1.0e-19;
    double P_MW  = P_loss_W * 1.0e-6;
    double eps   = p->a / p->R0;

    return 0.0562
         * pow(Ip_MA,    0.93)
         * pow(p->B0,    0.15)
         * pow(n19,      0.41)
         * pow(P_MW,    -0.69)
         * pow(p->R0,    1.97)
         * pow(p->kappa, 0.78)
         * pow(eps,      0.58)
         * pow(p->M,     0.19);
}

double p_lh_martin(const Params *p) {
    double n20 = p->n * 1.0e-20;
    double S   = 4.0 * PHYS_PI * PHYS_PI * p->R0 * p->a
               * sqrt((1.0 + p->kappa * p->kappa) / 2.0);
    double P_MW = 0.0488 * pow(n20, 0.717) * pow(p->B0, 0.803) * pow(S, 0.941);
    return P_MW * 1.0e6;
}