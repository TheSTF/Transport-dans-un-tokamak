#include <stdio.h>
#include "params.h"



Params params_machine(void) {

    Params p = {0};
    
    p.a = 0.4;// [m]

    p.R0 = 1.0;//[m]

    p.B0 = 5.0;//[T]

    p.chi = 1.0;//valeur provisoire [m^2/s]

    p.delta = 0.0;//[-]

    p.Ip = 1.0e6;//[A]

    p.kappa =  1.7;//[-]

    p.M = 2.0; //deutérium pur [uma]

    p.n = 5.0e19;//[m^-3]

    p.P_nbi = 10e6 ;//[W]

    p.sigma_nbi = p.a/3.0 ;//[m]

    p.T_edge = 0.1 ; //[keV]

    p.Zeff = 1.0 ; // aucune impureté [-]


    return p;


}


Params params_iter(void){

    
    Params p = {0};
    
    p.a = 2.0;// [m]

    p.R0 = 6.2;//[m]

    p.B0 = 5.3;//[T]

    p.delta = 0.33;//[-]

    p.Ip = 15.0e6;//[A]

    p.kappa =  1.7;//[-]

    p.M = 2.5; // D-T 50/50 [uma]

    p.n = 1.014e20;//[m^-3]

  
     // Zeff, chi, T_edge, P_nbi, sigma_nbi à 0
   

  

  


    return p;



}



void params_print(const Params *p)
{
    printf("R0        = %g m\n",      p->R0);
    printf("a         = %g m\n",      p->a);
    printf("kappa     = %g\n",        p->kappa);
    printf("delta     = %g\n",        p->delta);
    printf("B0        = %g T\n",      p->B0);
    printf("Ip        = %g A\n",      p->Ip);
    printf("n         = %g m^-3\n",   p->n);
    printf("M         = %g uma\n",    p->M);
    printf("Zeff      = %g\n",        p->Zeff);
    printf("chi       = %g m^2/s\n",  p->chi);
    printf("T_edge    = %g keV\n",    p->T_edge);
    printf("P_nbi     = %g W\n",      p->P_nbi);
    printf("sigma_nbi = %g m\n",      p->sigma_nbi);
}