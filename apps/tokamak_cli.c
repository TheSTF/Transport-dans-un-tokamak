
#include <stdio.h>
#include "params.h"

int main(void)
{
    Params machine = params_machine();
    Params iter    = params_iter();

    printf("=== Machine ===\n");
    params_print(&machine);

     printf("=== ITER ===\n");
    params_print(&iter);

   

    return 0;
}