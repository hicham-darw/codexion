#include "codexion.h"


t_monitor	*initial_monitor(t_global *g)
{
	t_monitor	*monitor;
	int			i;

	monitor = ft_memalloc(sizeof(t_monitor));
	if (!monitor)
		return (NULL);
	monitor->coders = ft_memalloc(sizeof(t_coder *) * g->number_of_coders);
	if (!monitor->coders)
	{
		free(monitor);
		return (NULL);
	}
	i = 0;
	while (i < g->number_of_coders)
	{
		monitor->coders[i] = &g->coders[i];
		i += 1;
	}
	monitor->globals = g;
	return (monitor);
}