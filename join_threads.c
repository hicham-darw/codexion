/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:58:41 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/01 06:41:47 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	join_all(t_global *global_var, int start_coders)
{
	pthread_mutex_lock(&global_var->mutex_stop);
	global_var->stop = 1;
	pthread_mutex_unlock(&global_var->mutex_stop);
	while (++start_coders < global_var->number_of_coders)
		pthread_join(global_var->coders[start_coders].thread, NULL);
	pthread_join(global_var->manager->thread, NULL);
}

int	join_manager(t_global *global_var)
{
	if (pthread_join(global_var->manager->thread, NULL))
	{
		join_all(global_var, -1);
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
		free(global_var);
		return (0);
	}
	return (1);
}

int	join_coders_at(t_coder *coders, int number_of_coders)
{
	t_global	*globals;
	int			i;

	globals = coders[0].globals;
	i = -1;
	while (++i < number_of_coders)
	{
		if (pthread_join(globals->coders[i].thread, NULL))
		{
			join_all(coders[i].globals, i);
			pthread_mutex_destroy(&globals->mutex_time);
			pthread_mutex_destroy(&globals->mutex_stop);
			free_global_var(globals);
			free(globals);
			return (0);
		}
	}
	return (1);
}

int	join_monitor(t_global *global_var)
{
	if (pthread_join(global_var->monitor->thread, NULL))
	{
		pthread_mutex_lock(&global_var->mutex_stop);
		global_var->stop = 1;
		pthread_mutex_unlock(&global_var->mutex_stop);
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
		free(global_var);
		return (0);
	}
	return (1);
}

int	join_threads(t_global *global_var)
{
	if (!join_manager(global_var))
		return (0);
	if (!join_coders_at(global_var->coders, global_var->number_of_coders))
		return (0);
	if (!join_monitor(global_var))
		return (0);
	return (1);
}
