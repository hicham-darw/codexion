#include "codexion.h"


t_monitor   *initial_monitor(t_global *global_var)
{
    t_monitor *monitor;
    int     i;

    monitor = malloc(sizeof(t_monitor));
    if (!monitor)
        return (NULL);
    memset(monitor, 0, sizeof(t_monitor));
    monitor->coders = (t_coder **)malloc(sizeof(t_coder *) * global_var->number_of_coders);
    if (!monitor->coders)
    {
        free(monitor);
        return (NULL);
    }
    i = 0;
    while (i < global_var->number_of_coders)
    {
        monitor->coders[i] = &global_var->coders[i];
        i += 1;
    }
    monitor->globals = global_var;

    return (monitor);
}