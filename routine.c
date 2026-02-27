#include "codexion.h"


void    *start_routine(void *args)
{
    t_coder *coder = (t_coder *)args;
    printf("start_routine.....\ncoder id: %d\n", coder->id);
    return NULL;
}