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
    if (gettimeofday(coder->globals->start_time, NULL) != 0)
        return NULL;
    if(coder->id % 2 == 0)
    {
        if (!coder->left_dongle->is_taken)
            coder->left_dongle->is_taken = 1;
        if (!coder->right_dongle->is_taken)
            coder->right_dongle->is_taken = 1;
        coder->is_compiling = 1;
        printf("ID: %d is compiling...!\n", coder->id);
        usleep(coder->globals->time_to_compile);
        coder->left_dongle->is_taken = 0;
        coder->right_dongle->is_taken = 0;
        gettimeofday(coder->last_compile_time, NULL);
        usleep(coder->globals->time_to_debug);
        printf("ID: %d is debugging...!\n", coder->id);
        usleep(coder->globals->time_to_refactor);
        printf("ID: %d is refactoring...!\n", coder->id);

    }
    else
    {
        usleep(coder->globals->time_to_compile);
        while (1)
        {
            if (coder->left_dongle->is_taken);
                continue;
            else
                break;
        }
        coder->left_dongle->is_taken = 1;
        printf("coder ID: %d taken left dongle....!\n", coder->id);
        while (1)
        {
            if (coder->right_dongle->is_taken)
                continue;
            else
                break;
        }
        coder->right_dongle->is_taken = 1;
        printf("coder ID: %d taken right dongle....!\n", coder->id);
        coder->is_compiling = 1;
        printf("ID: %d is compiling...!\n", coder->id);
        usleep(coder->globals->time_to_compile);
        gettimeofday(coder->last_compile_time, NULL);
        coder->left_dongle->is_taken = 0;
        coder->right_dongle->is_taken = 0;
        coder->is_compiling = 0;
        coder->is_debugging = 1;
        coder->is_refactoring = 0;
        gettimeofday(coder->left_dongle->last_compile, NULL);        
        gettimeofday(coder->right_dongle->last_compile, NULL);
        // waiting dongle to finish cooldown time ....        
        printf("ID: %d is debugging...!\n", coder->id);
        usleep(coder->globals->time_to_debug);
        coder->is_compiling = 0;
        coder->is_debugging = 1;
        coder->is_refactoring = 0;
        printf("ID: %d is refactoring...!\n", coder->id);
        usleep(coder->globals->time_to_refactor);
    }
    return NULL;
}