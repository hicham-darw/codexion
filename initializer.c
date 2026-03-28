#include "codexion.h"

int	initial_dongles_and_coders_and_heap(t_global **global_var)
{
	(*global_var)->dongles = initial_dongles((*global_var)->number_of_coders);
	if (!(*global_var)->dongles)
	{
		free_global_var((*global_var));
		return (0);
	}
	(*global_var)->coders = initial_coders(global_var);
	if (!(*global_var)->coders)
	{
		free_global_var((*global_var));
		return (0);
	}
	(*global_var)->heap = initial_heap(global_var);
	if (!(*global_var)->heap)
	{
		free_global_var((*global_var));
		return (0);
	}
	return (1);
}

t_manager	*initial_manager(t_global *global)
{
	t_manager	*manager;
	int			i;

	manager = (t_manager *)ft_memalloc(sizeof(t_manager));
	if (!manager)
		return (NULL);
	manager->dongles = (t_dongle **)ft_memalloc(sizeof(t_dongle *)
			* global->number_of_coders);
	if (!manager->dongles)
	{
		free(manager);
		return (NULL);
	}
	i = 0;
	while (i < global->number_of_coders)
	{
		manager->dongles[i] = &(global->dongles[i]);
		i += 1;
	}
	manager->heap = global->heap;
	manager->globals = global;
	return (manager);
}

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

int	initializer(t_global **global_var)
{
	if (!initial_dongles_and_coders_and_heap(global_var))
		return (0);
	(*global_var)->manager = initial_manager((*global_var));
	if (!(*global_var)->manager)
	{
		free_global_var((*global_var));
		return (0);
	}
	(*global_var)->monitor = initial_monitor(*global_var);
	if (!(*global_var)->monitor)
	{
		free_global_var(*global_var);
		return (0);
	}
	pthread_mutex_init(&(*global_var)->mutex_print, NULL);
	pthread_mutex_init(&(*global_var)->mutex_time, NULL);
	pthread_mutex_init(&(*global_var)->mutex_stop, NULL);
	return (1);
}
