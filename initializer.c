# include "codexion.h"


int     initializer(t_global **global_var)
{
    (*global_var)->dongles = initial_dongles((*global_var)->number_of_coders);
    if (!(*global_var)->dongles)
    {
        free_global_var((*global_var));
        return (0);
    }
    (*global_var)->coders = initial_coders(global_var);
    if (!(*global_var)->coders)
    {
        free_global_var((*global_var));
        return (0);
    }

    (*global_var)->manager = initial_manager((*global_var));
    if (!(*global_var)->manager)
    {
        free_global_var((*global_var));
        return (0);
    }
    (*global_var)->heap = (*global_var)->manager->heap;

    (*global_var)->monitor = initial_monitor();
    if (!(*global_var)->monitor)
    {
        free_global_var(*global_var);
        return (0);
    }
    pthread_mutex_init(&(*global_var)->mutex_print, NULL);
    pthread_mutex_init(&(*global_var)->mutex_time, NULL);
    return (1);
}