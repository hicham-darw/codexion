#include "codexion.h"

void free_global_var(t_global *global_var)
{
    t_global tmp;

    if (!global_var)
        return;
    if (global_var->schedular)
    {
        free(global_var->schedular);
        global_var->schedular = NULL;
    }
    free_dongles(global_var->dongles);
    global_var->dongles = NULL;
    free_coders(global_var->coders);
    global_var->coders = NULL;
    free(global_var);
}

void    free_dongles(t_dongle *dongles)
{
    if (!dongles)
        return ;
    free(dongles);
}

void    free_coders(t_coder *coder)
{
    if (!coder)
        return;
    free(coder);
}