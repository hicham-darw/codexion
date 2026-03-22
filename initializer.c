# include "codexion.h"

int initial_dongles_and_coders_and_heap(t_global **global_var)
{
    int     i;

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

    (*global_var)->heap = malloc(sizeof(t_heap));
    if (!(*global_var)->heap)
    {
        free_global_var((*global_var));
        return (0);
    }
    (*global_var)->heap->coders = (t_coder **)malloc(sizeof(t_coder *) * (*global_var)->number_of_coders);
    if (!(*global_var)->heap->coders)
    {
        free_global_var((*global_var));
        return (0);
    }
    i = 0;
    while (i < (*global_var)->number_of_coders)
    {
        (*global_var)->heap->coders[i] = &(*global_var)->coders[i];
        i += 1;
    }
    (*global_var)->heap->size = 0;
    (*global_var)->heap->capacity = (*global_var)->number_of_coders;
    pthread_mutex_init(&(*global_var)->heap->mutex_heap, NULL);    

    return (1);
}

int     initializer(t_global **global_var)
{

    if (!initial_dongles_and_coders_and_heap(global_var))
        return (0);

    (*global_var)->manager = initial_manager((*global_var));
    if (!(*global_var)->manager)
    {
        free_global_var((*global_var));
        return (0);
    }

    (*global_var)->monitor = initial_monitor(*global_var);
    if (!(*global_var)->monitor)
    {
        free_global_var(*global_var);
        return (0);
    }

    pthread_mutex_init(&(*global_var)->mutex_print, NULL);
    pthread_mutex_init(&(*global_var)->mutex_time, NULL);
    pthread_mutex_init(&(*global_var)->mutex_stop, NULL);
    return (1);
}