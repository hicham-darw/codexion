/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 02:01:16 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/31 23:23:33 by hel-hamo         ###   ########.fr       */
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
		return (0);
	}
	return (1);
}

int	create_coders_thread(t_global *global_var)
{
	int		i;

	i = -1;
	while (++i < global_var->number_of_coders)
	{
		if (pthread_create(&global_var->coders[i].thread, NULL,
				coder_routine, &global_var->coders[i]))
		{
			pthread_join(global_var->manager->thread, NULL);
			join_coders_at(global_var->coders, i);
			pthread_mutex_destroy(&global_var->mutex_print);
			pthread_mutex_destroy(&global_var->mutex_time);
			pthread_mutex_destroy(&global_var->mutex_stop);
			free_global_var(global_var);
			return (0);
		}
	}
	return (1);
}

int	create_monitor_thread(t_global *global_var)
{
	t_monitor	*monitor;

	monitor = global_var->monitor;
	if (pthread_create(&monitor->thread, NULL, monitor_routine, monitor))
	{
		pthread_join(global_var->manager->thread, NULL);
		join_coders_at(global_var->coders, global_var->number_of_coders);
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
		return (0);
	}
	return (1);
}

int	create_threads(t_global *global_var)
{
	if (!create_manager_thread(global_var))
		return (0);
	pthread_mutex_lock(&global_var->mutex_time);
	global_var->start_time = get_time_by_milisecond();
	pthread_mutex_unlock(&global_var->mutex_time);
	if (!create_coders_thread(global_var))
		return (0);
	if (!create_monitor_thread(global_var))
		return (0);
	return (1);
}
