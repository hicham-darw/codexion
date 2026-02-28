#include "codexion.h"


t_monitor   *initial_monitor(void)
{
    t_monitor *monitor;

    monitor = malloc(sizeof(t_monitor));
    if (!monitor)
        return (NULL);
    monitor->is_stop = 0;
    return (monitor);
}