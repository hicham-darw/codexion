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
    global_var->coders = NULL;

    free_dongles(global_var->dongles, global_var->number_of_coders);
    global_var->dongles = NULL;

    free(global_var->monitor);
    global_var->monitor = NULL;

    free_manager(global_var->manager);
    global_var->manager = NULL;

    free(global_var->heap);
    global_var->heap = NULL;

    pthread_mutex_destroy(&global_var->mutex_time);
    pthread_mutex_destroy(&global_var->mutex_print);
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

void    free_manager(t_manager *manager)
{
    if (!manager)
        return ;
    free(manager->dongles);
    free(manager->heap->coders);
    pthread_mutex_destroy(&manager->heap->mutex_heap);
    free(manager->heap);
    memset(manager, 0, sizeof(manager));
}