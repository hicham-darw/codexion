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
        usleep(coder->globals->time_to_compile);
        printf("coder ID: %d is compiled!\n", coder->id);
        usleep(coder->globals->time_to_debug);
        printf("coder ID: %d is debugged!\n", coder->id);
        usleep(coder->globals->time_to_refactor);
        printf("coder ID: %d is refactored!\n", coder->id);
        coder->is_debugging = 0;
        coder->is_refactoring = 0;
        printf("coder ID: %d is compiling debugging refactoring...\n", coder->id);
        printf("is taken l_dongle: %d\n", coder->left_dongle->is_taken);
        printf("is taken r_dsongle: %d\n", coder->right_dongle->is_taken);
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