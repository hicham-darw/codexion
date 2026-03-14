#include "codexion.h"


void    *monitor_routine(void *args)
{
    return (NULL);
}


void release_dongles(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;

    left = coder->left_dongle;
    right = coder->right_dongle;
    
    left->is_taken = 0;
    right->is_taken = 0;
    pthread_cond_signal(&left->cond_dongle);
    pthread_mutex_unlock(&left->mutex_dongle);
    pthread_mutex_unlock(&right->mutex_dongle);
}

void take_dongles(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;

    left = coder->left_dongle;
    right = coder->right_dongle;
    if (!(coder->id % 2))
    {
        pthread_mutex_lock(&left->mutex_dongle);
        pthread_mutex_lock(&right->mutex_dongle);
        if (left->is_taken || right->is_taken)
        {
            pthread_mutex_unlock(&left->mutex_dongle);
            pthread_mutex_unlock(&right->mutex_dongle);
            while (left->is_taken || right->is_taken)
                pthread_cond_wait(&left->cond_dongle, &left->mutex_dongle);
        }
        else
        {
            left->is_taken = 1;
            right->is_taken = 1;
            pthread_mutex_lock(&coder->globals->mutex_print);
            printf("coder %d is took left\n", coder->id);
            printf("coder %d is took right\n", coder->id);
            pthread_mutex_unlock(&coder->globals->mutex_print);
        }

    }
    else
    {
        pthread_mutex_lock(&right->mutex_dongle);
        pthread_mutex_lock(&left->mutex_dongle);

        if (right->is_taken || left->is_taken)
        {
            pthread_mutex_unlock(&right->mutex_dongle);
            pthread_mutex_unlock(&left->mutex_dongle);
            while (left->is_taken || right->is_taken)
                pthread_cond_wait(&left->cond_dongle, &left->mutex_dongle);
        }
        left->is_taken = 1;
        right->is_taken = 1;
        pthread_mutex_lock(&coder->globals->mutex_print);
        printf("coder %d is took left\n", coder->id);
        printf("coder %d is took right\n", coder->id);
        pthread_mutex_unlock(&coder->globals->mutex_print);
        
    }
}

void    *start_routine(void *args)
{
    t_coder *coder ;
    int     i;

    coder = (t_coder *)args;
    if (!coder)
        return NULL;
    insert_coder_to_heap(&coder->globals->manager->heap, coder);
    
    pthread_mutex_lock(&coder->globals->mutex_time);
    coder->last_compile_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->globals->mutex_time);
    

    while (1)
    {

        //after entering to heap should wait manager to signaling them

        // start compile -> debug ----> refactor
        // take_dongles(coder);
        pthread_mutex_lock(&coder->mutex_coder);
        while (!coder->both_available)
            pthread_cond_wait(&coder->cond_coder, &coder->mutex_coder);
        coder->both_available = 0;
        pthread_mutex_unlock(&coder->mutex_coder);

        pthread_mutex_lock(&coder->globals->mutex_print);
        printf("Coder %d is compiling\n", coder->id);
        pthread_mutex_unlock(&coder->globals->mutex_print);
        
        usleep(coder->globals->time_to_compile * 1000);
        
        // release_dongles(coder);

        pthread_mutex_lock(&coder->globals->mutex_print);
        printf("Coder %d released dongles\n", coder->id);
        pthread_mutex_unlock(&coder->globals->mutex_print);

        pthread_mutex_lock(&coder->globals->mutex_print);
        printf("Coder %d is debugging\n", coder->id);
        pthread_mutex_unlock(&coder->globals->mutex_print);

        usleep(coder->globals->time_to_debug * 1000);

        pthread_mutex_lock(&coder->globals->mutex_print);
        printf("Coder %d is refactoring\n", coder->id);
        pthread_mutex_unlock(&coder->globals->mutex_print);
        
        usleep(coder->globals->time_to_refactor * 1000);


    }
    return NULL;
}
