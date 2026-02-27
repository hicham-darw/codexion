#include "codexion.h"


void    *start_routine(void *args)
{
    t_coder *coder ;
    int     i;

    coder = (t_coder *)args;
    if (!coder)
        return NULL;
    if (gettimeofday(coder->globals->start_time, NULL) != 0)
        return NULL;
    if(coder->globals->number_of_coders % 2 == 0)
    {
        i = 0;
        while (i < coder->globals->number_of_coders)
        {
            if (i % 2 == 0)
            {
                //sstake_dongle()
            }
            else{
                //waiting_dongle_and_cooldown
            }
            i += 1;
        }        
        even_coders_take_dongles();
        odd_coders_take_dongles();
    }
    return NULL;
}