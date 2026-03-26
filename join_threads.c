#include "codexion.h"

int    join_manager(t_global *global_var)
{
    if (pthread_join(global_var->manager->thread, NULL))
    {
        pthread_mutex_destroy(&global_var->mutex_print);
        pthread_mutex_destroy(&global_var->mutex_time);
        pthread_mutex_destroy(&global_var->mutex_stop);
        free_global_var(global_var);
        return (0);
    }
    return (1);
}

int     join_coders(t_global *global_var)
{
    int i;

    i = -1;
    while (++i < global_var->number_of_coders)
    {
        if (pthread_join(global_var->coders[i].thread, NULL))
        {
            pthread_mutex_destroy(&global_var->mutex_print);
            pthread_mutex_destroy(&global_var->mutex_time);
            pthread_mutex_destroy(&global_var->mutex_stop);
            free_global_var(global_var);
            return (0);
        }
    }
    return (1);
}

int     join_monitor(t_global *global_var)
{
    if (pthread_join(global_var->monitor->thread, NULL))
    {
        pthread_mutex_destroy(&global_var->mutex_print);
        pthread_mutex_destroy(&global_var->mutex_time);
        pthread_mutex_destroy(&global_var->mutex_stop);
        free_global_var(global_var);
        return (0);
    }
    return (1);
}

int     join_threads(t_global *global_var)
{
    if (!join_manager(global_var))
        return (0);
    if (!join_coders(global_var))
        return (0);
    if (!join_monitor(global_var))
        return (0);

    return (1);
}