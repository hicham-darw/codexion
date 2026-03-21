#include "codexion.h"

void    *monitor_routine(void *args)
{
    t_monitor   *monitor;
    int     stop;
    int     i;

    monitor = (t_monitor *)args;
    // while (1)
    // {
    //     printf("number of coders: \n");
    //     printf("%p\n", monitor->globals);
    //     printf("ko!!-> %d\n", i);
    //     stop = 0;
    //     while (i < monitor->globals->number_of_coders)
    //     {
    //         pthread_mutex_lock(&monitor->coders[i]->mutex_coder);
    //         if (monitor->globals->number_of_compiles_required == monitor->coders[i]->total_compiling)
    //             stop += 1;
    //         if (
    //             get_time_by_milisecond() - monitor->coders[i]->last_compile_time > monitor->globals->time_to_burnout
    //         )
    //         {
    //             i = 0;
    //             while (i < monitor->globals->number_of_coders)
    //             {
    //                 pthread_mutex_lock(&monitor->coders[i]->mutex_coder);
    //                 monitor->coders[i]->stop = 1;
    //                 pthread_cond_broadcast(&monitor->coders[i]->cond_coder);
    //                 pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);

    //                 i ++ ;
    //             }
    //         }
    //         pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
    //         i ++;
    //     }
    //     if (stop == monitor->globals->number_of_coders)
    //         break;
    //         // usleep(500);
    // }
    // i = 0;
    // while (i < monitor->globals->number_of_coders)
    // {
    //     pthread_mutex_lock(&monitor->coders[i]->mutex_coder);
    //     monitor->coders[i]->stop = 1;
    //     pthread_cond_broadcast(&monitor->coders[i]->cond_coder);
    //     pthread_mutex_unlock(&monitor->coders[i]->mutex_coder);
    //     i += 1;
    // }
    return (NULL);
}
