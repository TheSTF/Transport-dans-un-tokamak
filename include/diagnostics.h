#ifndef TOKAMAK_DIAGNOSTICS_H
#define TOKAMAK_DIAGNOSTICS_H

#include "params.h"
#include "grid.h"

typedef struct {
    double W;      /* energie thermique stockee [J] */
    double P_ohm;  /* puissance ohmique [W] */
    double P_nbi;  /* puissance NBI [W] */
    double P_loss; /* pertes diffusives au bord [W] */
    double tau_E;  /* temps de confinement [s] */
    double H98;    /* facteur H98 [-] */
    double f_GW;   /* fraction de Greenwald [-] */
    double beta_N; /* beta normalise [%] */
    double q95;    /* facteur de securite a 95 % [-] */
    double nTtau;  /* triple produit [keV s m^-3] */
    double P_LH;   /* seuil L-H de Martin [W] */
} Diag;

/* Conversions d'unites */
double kev_to_joule(double T_keV);
double joule_to_kev(double T_J);

/* Diagnostics 0D (S1) */
double plasma_volume(const Params *p);               /* [m^3]  */
double greenwald_density(const Params *p);           /* [m^-3] */
double beta_max_troyon(const Params *p);             /* [%]    */
double q95_estimate(const Params *p);                /* [-]    */
double tau_ipb98(const Params *p, double P_loss_W);  /* [s]    */
double p_lh_martin(const Params *p);                 /* [W]    */

/* Bilan d'energie 2D (S3) */
void   diag_compute(Diag *d,
                    const Field *T, const Field *T_old,
                    const unsigned char *mask, const Params *p, double dt);

#endif /* TOKAMAK_DIAGNOSTICS_H */