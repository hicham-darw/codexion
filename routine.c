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
    return NULL;
    if(coder->id % 2 == 0)
    {
        if (!coder->left_dongle->is_taken)
            coder->left_dongle->is_taken = 1;
        if (!coder->right_dongle->is_taken)
            coder->right_dongle->is_taken = 1;
        coder->is_compiling = 1;
        printf("ID: %d is compiling...!", coder->id);
        usleep(coder->globals->time_to_compile);
        gettimeofday(coder->last_compile_time, NULL);
        usleep(coder->globals->time_to_debug);
        printf("ID: %d is debugging...!", coder->id);
        usleep(coder->globals->time_to_refactor);

    }
    else
    {
        usleep(coder->globals->time_to_compile);
        while (true)
        {
            if (gettimeofday(&timeval, NULL) != 0)
            {
                printf("error");
                return (NULL);
            }
            printf("timeval: %ld\n", timeval.tv_sec);
            if ((long int)timeval.tv_sec - (long int)coder->globals->start_time->tv_sec < coder->globals->time_to_compile)
                continue;
            if ((long int)timeval.tv_sec - (long int)coder->globals->start_time->tv_sec >= coder->globals->time_to_compile)
                break;
            printf("not NOw...\n %ld\n", (long int)timeval.tv_sec);
        }
        printf("im waited time of of compile my ID: %d\n", coder->id);

    }
    return NULL;
}