#include "codexion.h"


void    *monitor_routine(void *args)
{
    return (NULL);
}
// releasing both dongles should if cant release another dongle should put it to the desk!
void release_dongles(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;

    left = coder->left_dongle;
    right = coder->right_dongle;
    if (coder->id % 2){
        pthread_mutex_lock(&left->mutex_dongle);
        pthread_mutex_lock(&right->mutex_dongle);
    }
    else {
        pthread_mutex_lock(&right->mutex_dongle);
        pthread_mutex_lock(&left->mutex_dongle);
    }

    left->is_taken = 0;
    right->is_taken = 0;
    if (coder->id % 2)
        pthread_cond_signal(&right->cond_dongle);
    else
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
    if (coder->id % 2)
    {
        while (left->is_taken || right->is_taken)
            pthread_cond_wait(&right->cond_dongle, &right->mutex_dongle);
    }
    else
    {
        while (left->is_taken || right->is_taken)
            pthread_cond_wait(&left->cond_dongle, &left->mutex_dongle);
    }
    right->is_taken = 1;
    left->is_taken = 1;
    pthread_mutex_unlock(&left->mutex_dongle);
    pthread_mutex_unlock(&right->mutex_dongle);
}

void    *start_routine(void *args)
{
    t_dongle    *first, *last;
    t_coder *coder ;
    struct timeval timeval;
    int     i;

    coder = (t_coder *)args;
    if (!coder)
        return NULL;

    while (1)
    {
        take_dongles(coder);
        printf("Coder %d took left dongle\n", coder->id);
        printf("Coder %d took right dongle\n", coder->id);

        printf("Coder %d is compiling\n", coder->id);
        sleep(5);
        release_dongles(coder);
        // printf("Coder %d finished compiling\n", coder->id);
        // if (coder->id % 2){
        //     release_dongle(coder->left_dongle);
        //     release_dongle(coder->right_dongle);
        // } else {
        //     release_dongle(coder->right_dongle);
        //     release_dongle(coder->left_dongle);
        // }
        printf("Coder %d released dongles\n", coder->id);

        printf("Coder %d is debugging\n", coder->id);
        sleep(5);

        printf("Coder %d is refactoring\n", coder->id);
        sleep(5);


    }
    return NULL;
}