#include "codexion.h"


void    *monitor_routine(void *args)
{
    return (NULL);
}
// releasing both dongles should if cant release another dongle should put it to the desk!
void release_dongles(t_coder *coder)
{
    pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
    pthread_mutex_lock(&coder->right_dongle->mutex_dongle);

    coder->left_dongle->is_taken = 0;
    coder->right_dongle->is_taken = 0;

    pthread_cond_signal(&coder->left_dongle->cond_dongle);
    pthread_cond_signal(&coder->right_dongle->cond_dongle);

    pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
    pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
}

void take_dongles(t_coder *coder)
{
    pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
    pthread_mutex_lock(&coder->right_dongle->mutex_dongle);

    while (coder->left_dongle->is_taken)
        pthread_cond_wait(&coder->left_dongle->cond_dongle, &coder->left_dongle->mutex_dongle);
    while (coder->right_dongle->is_taken)
        pthread_cond_wait(&coder->right_dongle->cond_dongle, &coder->right_dongle->mutex_dongle);

    coder->left_dongle->is_taken = 1;
    coder->right_dongle->is_taken = 1;

    pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
    pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
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
    if (coder->globals->number_of_coders % 2)
        usleep(100);
    while (1)
    {
        if (coder->id % 2)
        {
            take_dongles(coder);
            printf("Coder %d took left dongle\n", coder->id);
            printf("Coder %d took right dongle\n", coder->id);
        }
        else
        {
            take_dongles(coder);
            printf("Coder %d took right dongle\n", coder->id);
            printf("Coder %d took left dongle\n", coder->id);
        }

        printf("Coder %d is compiling\n", coder->id);
        sleep(3);
        // printf("Coder %d finished compiling\n", coder->id);
        release_dongles(coder);
        printf("Coder %d released dongles\n", coder->id);

        printf("Coder %d is debugging\n", coder->id);
        sleep(5);

        printf("Coder %d is refactoring\n", coder->id);
        sleep(5);


    }
    return NULL;
}