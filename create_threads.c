/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 02:01:16 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/12 14:23:38 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_manager_thread(t_global *global_var)
{
	t_manager	*manager;

	manager = global_var->manager;
	if (pthread_create(&manager->thread, NULL, manager_routine, manager))
	{
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	return (TRUE);
}

int	create_coders_thread(t_global *global_var)
{
	t_coder		*coders;
	long		i;

	coders = global_var->coders;
	i = -1;
	while (++i < global_var->number_of_coders)
	{
		if (pthread_create(&coders[i].thread, NULL,
				coder_routine, &coders[i]))
		{
			change_stop_var(global_var->monitor);
			pthread_join(global_var->manager->thread, NULL);
			while (--i >= 0)
				pthread_join(coders[i].thread, NULL);
			pthread_mutex_destroy(&global_var->mutex_print);
			pthread_mutex_destroy(&global_var->mutex_time);
			pthread_mutex_destroy(&global_var->mutex_stop);
			free_global_var(global_var);
			free(global_var);
			return (FALSE);
		}
	}
	return (TRUE);
}

int	create_monitor_thread(t_global *global_var)
{
	t_monitor	*monitor;
	long		i;

	monitor = global_var->monitor;
	if (pthread_create(&monitor->thread, NULL, monitor_routine, monitor))
	{
		change_stop_var(global_var->monitor);
		pthread_join(global_var->manager->thread, NULL);
		i = -1;
		while (++i < global_var->number_of_coders)
			pthread_join(global_var->coders[i].thread, NULL);
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	return (TRUE);
}

int	create_threads(t_global *global_var)
{
	if (!create_manager_thread(global_var))
		return (FALSE);
	global_var->start_time = get_time_by_milisecond();
	if (global_var->start_time == -1)
	{
		change_stop_var(global_var->monitor);
		pthread_join(global_var->manager->thread, NULL);
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	if (!create_coders_thread(global_var))
		return (FALSE);
	if (!create_monitor_thread(global_var))
		return (FALSE);
	return (TRUE);
}
