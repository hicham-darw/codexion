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
    free_coders(global_var->coders, global_var->number_of_coders);

    free_dongles(global_var->dongles, global_var->number_of_coders);

    free_heap(global_var->heap);

    free_manager(global_var->manager);
    
    free_monitor(global_var->monitor);

    free(global_var);
}

void    free_dongles(t_dongle *dongles, int number_of_coders)
{
    int     i;

    if (!dongles || !number_of_coders)
        return ;
    i = 0;
    while (i < number_of_coders)
        pthread_mutex_destroy(&dongles[i++].mutex_dongle);
    free(dongles);
    dongles = NULL;
}

void    free_heap(t_heap *heap)
{
    if (!heap)
        return;

    free(heap->coders);
    heap->coders = NULL;
    pthread_mutex_destroy(&heap->mutex_heap);

    free(heap);
    heap = NULL;

}
void    free_coders(t_coder *coders, int number_of_coders)
{
    int     i;

    if (!coders || !number_of_coders)
        return ;
    i = 0;
    while (i < number_of_coders)
    {
        pthread_mutex_destroy(&coders[i].mutex_coder);
        pthread_cond_destroy(&coders[i].cond_coder);
        i += 1;
    }
    free(coders);
    coders = NULL;
}

void    free_monitor(t_monitor *monitor)
{
    if (!monitor)
        return;

    free(monitor->coders);
    monitor->coders = NULL;

    free(monitor);
    monitor = NULL;
}

void    free_manager(t_manager *manager)
{
    if (!manager)
        return ;
    free(manager->dongles);
    manager->dongles = NULL;
    
    free(manager);
    manager = NULL;
}