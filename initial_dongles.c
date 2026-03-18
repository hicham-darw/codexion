#include "codexion.h"

t_dongle    *initial_dongles(int number_of_coders)
{
    t_dongle *dongle;
    int     i;

    dongle = malloc(sizeof(t_dongle) * (number_of_coders));
    if (!dongle)
        return (NULL);
    i = 0;
    while (i < number_of_coders)
    {
        dongle[i].id = i + 1;
        dongle[i].is_taken = 0;
        pthread_mutex_init(&dongle[i].mutex_dongle, NULL);
        i += 1;
    }
    return (dongle);
}