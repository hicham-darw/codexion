#include "codexion.h"

int     join_threads(t_global *global_var)
{
    int     i;
    if (pthread_join(global_var->manager->thread, NULL))
    {
        pthread_mutex_destroy(&global_var->mutex_print);
        pthread_mutex_destroy(&global_var->mutex_time);
        free_global_var(global_var);
        return (0);
    }
    i = -1;
    while (++i < global_var->number_of_coders)
    {
        if (pthread_join(global_var->coders[i].thread, NULL))
        {
            pthread_mutex_destroy(&global_var->mutex_print);
            pthread_mutex_destroy(&global_var->mutex_time);
            free_global_var(global_var);
            return (0);
        }
    }
    if (pthread_join(global_var->monitor->thread, NULL))
    {
        pthread_mutex_destroy(&global_var->mutex_print);
        pthread_mutex_destroy(&global_var->mutex_time);
        free_global_var(global_var);
        return (0);
    }
    return 1;
}