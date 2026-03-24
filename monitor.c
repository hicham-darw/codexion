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


void    *monitor_routine(void *args)
{
    t_monitor   *monitor;
    int     finished_compile;
    int     i;
    int     burned_out;

    monitor = (t_monitor *)args;

    burned_out = 0;
    while (1)
    {
        i = 0;
        finished_compile = 0;
        while (i < monitor->globals->number_of_coders)
        {
            pthread_mutex_lock(&monitor->coders[i]->mutex_coder);
            if (monitor->globals->number_of_compiles_required == monitor->coders[i]->total_compiling)
                finished_compile += 1;
            if (
                get_time_by_milisecond() - monitor->coders[i]->last_compile_time > monitor->globals->time_to_burnout
                && (monitor->coders[i]->last_compile_time != 0)
            )
            {
                burned_out = 1;
                pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
                break;
            }
            pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
            i++;
        }
        if (burned_out)
        {
            print_log(monitor->coders[i], "is burned out!!!!!");
            break;
        }
        if (finished_compile == monitor->globals->number_of_coders)
            break;
        usleep(500);
    }

    pthread_mutex_lock(&monitor->globals->mutex_stop);
    monitor->globals->stop = 1;
    pthread_mutex_unlock(&monitor->globals->mutex_stop);

    i = 0;
    while (i < monitor->globals->number_of_coders)
    {
        pthread_mutex_lock(&monitor->coders[i]->mutex_coder);
        pthread_cond_broadcast(&monitor->coders[i]->cond_coder);
        pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
        i++;
    }

    return (NULL);
}
