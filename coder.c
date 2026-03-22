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

void    get_start_time_of_coders(t_coder *coder)
{
    // pthread_mutex_lock(&coder->mutex_coder);
    coder->start_time = get_time_by_milisecond();
    // pthread_mutex_unlock(&coder->mutex_coder);
}


void    waiting_to_compile(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    while (!coder->can_compile)
        pthread_cond_wait(&coder->cond_coder, &coder->mutex_coder);
    coder->can_compile = 0;
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


void    get_last_compile_time(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    coder->last_compile_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->mutex_coder);
}


void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    if ((coder->id % 2))
        usleep(400);
    pthread_mutex_lock(&coder->globals->mutex_time);
    if (!coder->globals->start_time)
        coder->globals->start_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->globals->mutex_time);

    coder->last_compile_time = coder->globals->start_time;
    while (1)
    {        
        insert_coder_to_heap(coder->globals->heap, coder);
        
        pthread_mutex_lock(&coder->globals->mutex_stop);
        if (coder->globals->stop)
        {
            release_dongles(coder);
            pthread_mutex_unlock(&coder->globals->mutex_stop);
            return (NULL);
        }
        // printf("stop of globals: %d\n", coder->globals->stop);
        // printf("coder->id: %d\n", coder->id);
        pthread_mutex_unlock(&coder->globals->mutex_stop);
        
        waiting_to_compile(coder);
        
        start_compiling(coder);
        get_last_compile_time(coder);

        release_dongles(coder);        
        increment_total_compiling(coder);
        
        start_debugging(coder);
        
        start_refactoring(coder);
    }

    return (NULL);
}