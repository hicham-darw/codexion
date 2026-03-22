#include "codexion.h"

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
                print_log(monitor->coders[i], "is burned out!");
                burned_out = 1;
                pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
                break;
            }
            pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
            i++;
        }
        if (burned_out)
            break;
        if (finished_compile == monitor->globals->number_of_coders)
            break;
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
