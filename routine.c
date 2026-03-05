#include "codexion.h"


void    *monitor_routine(void *args)
{
    return (NULL);
}

void    *start_routine(void *args)
{
    t_coder *coder ;
    struct timeval timeval;
    int     i;

    coder = (t_coder *)args;
    if (!coder)
        return NULL;
    coder->globals->start_time = get_time_by_milisecond();
    printf("TIME: %ld\n", coder->globals->start_time);    
    pthread_mutex_lock(&coder->left_dongle->lock);
    pthread_mutex_lock(&coder->right_dongle->lock);
    coder->left_dongle->is_taken = 1;
    coder->right_dongle->is_taken = 1;
    printf("coder ID: %d -> is compiling...\n", coder->id);
    usleep(coder->globals->time_to_compile * 1000);
    printf("coder ID: %d -> is deugging...\n", coder->id);
    usleep(coder->globals->time_to_debug * 1000);
    printf("coder ID: %d -> is refactoring...\n", coder->id);
    usleep(coder->globals->time_to_refactor * 1000);
    pthread_mutex_unlock(&coder->right_dongle->lock);
    pthread_mutex_unlock(&coder->left_dongle->lock);
    // if(coder->id % 2 == 0)
    // {
    //     if (!coder->left_dongle->is_taken)
    //         coder->left_dongle->is_taken = 1;
    //     if (!coder->right_dongle->is_taken)
    //         coder->right_dongle->is_taken = 1;
    //     printf("coder ID: %d is taken left dongle ID: %d\n", coder->id, coder->left_dongle->id);
    //     printf("coder ID: %d is taken right dongle ID: %d\n", coder->id, coder->right_dongle->id);
    //     coder->is_compiling = 1;
    //     coder->is_debugging = 0;
    //     coder->is_refactoring = 0;
    //     printf("ID: %d is compiling...!\n", coder->id);
    //     usleep(coder->globals->time_to_compile * 10000);
    //     printf("finished compiling: check odd!\n");
    //     gettimeofday(&coder->last_compile_time, NULL);
    //     coder->left_dongle->is_taken = 0;
    //     coder->right_dongle->is_taken = 0;
    //     coder->is_compiling = 0;
    //     coder->is_debugging = 1;
    //     coder->is_refactoring = 0;
    //     printf("ID: %d is debugging...!\n", coder->id);
    //     usleep(coder->globals->time_to_debug * 10000);
    //     coder->is_compiling = 0;
    //     coder->is_debugging = 0;
    //     coder->is_refactoring = 1;
    //     printf("ID: %d is refactoring...!\n", coder->id);
    //     usleep(coder->globals->time_to_refactor * 1000);
    //     coder->is_refactoring = 0;

    // }
    // else
    // {
    //     usleep(100);
    //     while (coder->left_dongle->is_taken || coder->right_dongle->is_taken)
    //         usleep(100);
    //     coder->left_dongle->is_taken = 1;
    //     printf("coder ID: %d taken left dongle....!\n", coder->id);
    //     coder->right_dongle->is_taken = 1;
    //     printf("coder ID: %d taken right dongle....!\n", coder->id);
    //     coder->is_compiling = 1;
    //     coder->is_debugging = 0;
    //     coder->is_refactoring = 0;
    //     printf("ID: %d is compiling...!\n", coder->id);
    //     usleep(coder->globals->time_to_compile * 1000);
    //     coder->left_dongle->last_compile = malloc(sizeof(struct timeval));
    //     if (!coder->left_dongle->last_compile)
    //         return (NULL);
    //     gettimeofday(&coder->last_compile_time, NULL);
    //     coder->left_dongle->is_taken = 0;
    //     coder->right_dongle->is_taken = 0;
    //     coder->is_compiling = 0;
    //     coder->is_debugging = 1;
    //     coder->is_refactoring = 0;
    //     gettimeofday(coder->left_dongle->last_compile, NULL);        
    //     gettimeofday(coder->right_dongle->last_compile, NULL);
    //     printf("ID: %d is debugging...!\n", coder->id);
    //     coder->is_compiling = 0;
    //     coder->is_debugging = 1;
    //     coder->is_refactoring = 0;
    //     usleep(coder->globals->time_to_debug * 1000);
    //     printf("ID: %d is refactoring...!\n", coder->id);
    //     usleep(coder->globals->time_to_refactor * 1000);
    // }
    return NULL;
}