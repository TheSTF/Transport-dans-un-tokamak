#ifndef PARAMS_H
#define PARAMS_H

typedef struct t_params{
    
    double R0; /* grand rayon [m] */
    double a; /*petit rayon [m]*/
    double kappa; /*allongement [-]*/
    double delta; /* triangularité [-]*/
    double B0; /* champ toroidal sur l'axe [T]*/
    double Ip; /* courant plasma [A] */
    double n; /* densité [m^-3]*/
    double M; /* masse ionique moyenne [uma]: 2 (D pur), 2.5 (D-T)*/
    double Zeff; /* charge effective [-] : 1 pour un plasma pur*/
    double chi; /*Diffusivité thermique (m^2/s)*/
    double T_edge; /*temperature au bord [keV] */
    double P_nbi_nom; /*puissance NBI [W]*/
    double sigma_nbi; /*largeur du depot NBI [m]*/




} Params;

Params params_machine(void);
Params params_iter(void);
void params_print(const Params *p);
#endif