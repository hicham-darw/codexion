#include "codexion.h"


void    *monitor_routine(void *args)
{
    return (NULL);
}

void swap_coders(t_coder **a, t_coder **b)
{
    t_coder *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

void release_dongles(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;

    left = coder->left_dongle;
    right = coder->right_dongle;
    
    left->is_taken = 0;
    right->is_taken = 0;
    pthread_mutex_unlock(&left->mutex_dongle);
    pthread_mutex_unlock(&right->mutex_dongle);
}

void take_dongles(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;

    left = coder->left_dongle;
    right = coder->right_dongle;
    if (coder->id % 2)
    {
        pthread_mutex_lock(&left->mutex_dongle);
        pthread_mutex_lock(&right->mutex_dongle);        
    }
    else
    {
        pthread_mutex_lock(&right->mutex_dongle);
        pthread_mutex_lock(&left->mutex_dongle);
    }
    left->is_taken = 1;
    right->is_taken = 1;
    pthread_mutex_lock(&coder->globals->mutex_print);
    printf("coder %d is took left\n", coder->id);
    printf("coder %d is took right\n", coder->id);
    pthread_mutex_unlock(&coder->globals->mutex_print);


}

void    put_dongles(t_coder *coder)
{
    pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
    pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    while (!coder->globals->is_finished)
    {
        // locking coder for store time of starting routine
        pthread_mutex_lock(&coder->mutex_coder);
        if (!coder->number_of_compilling)
            coder->start_time = get_time_by_milisecond();
        pthread_mutex_unlock(&coder->mutex_coder);

        // insert_coder_to_heap()
        insert_coder_to_heap(coder->globals->heap, coder);

        pthread_mutex_lock(&coder->mutex_coder);
        while (!coder->can_compile)
            pthread_cond_wait(&coder->cond_coder, &coder->mutex_coder);
        pthread_mutex_unlock(&coder->mutex_coder);

        print_log(coder, "is compiling");        
        precise_sleep(coder->globals->time_to_compile);
        
        coder->left_dongle->is_taken = 0;
        coder->right_dongle->is_taken = 0;
        pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
        pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);        

        // last_compile for FIFO and EDF
        pthread_mutex_lock(&coder->mutex_coder);
        coder->last_compile_time = get_time_by_milisecond();
        pthread_mutex_unlock(&coder->mutex_coder);
        

        //debugging....
        print_log(coder, "is debugging");
        precise_sleep(coder->globals->time_to_debug);

        // refactoring .....
        print_log(coder, "is refactoring");
        precise_sleep(coder->globals->time_to_refactor);
    }

    return NULL;
}