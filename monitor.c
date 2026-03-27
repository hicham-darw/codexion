#include "codexion.h"


t_monitor   *initial_monitor(t_global *global_var)
{
    t_monitor *monitor;
    int     i;

    monitor = ft_memalloc(sizeof(t_monitor));
    if (!monitor)
        return (NULL);
    monitor->coders = (t_coder **)ft_memalloc(sizeof(t_coder *) * global_var->number_of_coders);
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

static void    change_stop_var(t_monitor *monitor)
{
    pthread_mutex_lock(&monitor->globals->mutex_stop);
    monitor->globals->stop = 1;
    pthread_mutex_unlock(&monitor->globals->mutex_stop);
}

void    check_each_coder(t_coder *coder, int *finish_compile, int *burnout)
{
    pthread_mutex_lock(&coder->mutex_coder);
    if (
        coder->globals->number_of_coders == 1
        && coder->last_compile_time
        && burnout_coder(coder)
    )
    {
        *burnout = 1;
        pthread_mutex_unlock(&coder->mutex_coder);
        return;
    }
    if (coder->globals->number_of_compiles_required == coder->total_compiling)
        finish_compile += 1;
    // printf("coder->is_compiling: %d\n", coder->is_compiling);
    // printf("burnout_coder: %d\n", burnout_coder(coder));
    // printf("coder->last_compile: %ld\n", coder->last_compile_time);
    if (
        !coder->is_compiling
        && burnout_coder(coder)
        && (coder->last_compile_time != 0)
    )
    {  
        *burnout = 1;
        pthread_mutex_unlock(&coder->mutex_coder);
        return;
    }
    pthread_mutex_unlock(&coder->mutex_coder);
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
            check_each_coder(monitor->coders[i], &finished_compile, &burned_out);
            if (burned_out)
                break;
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

    change_stop_var(monitor);
    return (NULL);
}
