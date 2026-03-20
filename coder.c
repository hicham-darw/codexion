#include "codexion.h"


void release_dongles(t_coder *coder)
{
    if (coder->id % 2)
    {
        pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
        pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
    }
    else
    {
        pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
        pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
    }
    coder->left_dongle->is_taken = 0;
    coder->right_dongle->is_taken = 0;
    pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
    pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
}


void    waiting_to_compile(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    while (!coder->can_compile)
        pthread_cond_wait(&coder->cond_coder, &coder->mutex_coder);
    pthread_mutex_unlock(&coder->mutex_coder);
}

void    start_compiling(t_coder *coder)
{
    print_log(coder, "is compiling");        
    precise_sleep(coder->globals->time_to_compile);
}

void    start_debugging(t_coder *coder)
{
    print_log(coder, "is debugging");
    precise_sleep(coder->globals->time_to_debug);
}

void    start_refactoring(t_coder *coder)
{
    print_log(coder, "is refactoring");
    precise_sleep(coder->globals->time_to_refactor);
}

void    increment_total_compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    coder->total_compiling += 1;
    pthread_mutex_unlock(&coder->mutex_coder);
}

void    get_start_time_of_coders(t_coder *coder)
{

    pthread_mutex_lock(&coder->mutex_coder);
    if (!coder->total_compiling)
        coder->start_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->mutex_coder);

}

void    get_last_compile_time(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    coder->last_compile_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->mutex_coder);
}


void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    get_start_time_of_coders(coder);

    if (!(coder->id % 2))
        usleep(500);
    while (1)
    {
        insert_coder_to_heap(coder->globals->heap, coder);

        waiting_to_compile(coder);
        start_compiling(coder);

        release_dongles(coder);

        get_last_compile_time(coder);
        increment_total_compiling(coder);

        start_debugging(coder);
        start_refactoring(coder);
    }
    return (NULL);
}