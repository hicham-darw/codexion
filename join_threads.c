/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:58:41 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/31 23:44:05 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	join_manager(t_global *global_var)
{
	if (pthread_join(global_var->manager->thread, NULL))
	{
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
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
			pthread_mutex_destroy(&globals->mutex_print);
			pthread_mutex_destroy(&globals->mutex_time);
			pthread_mutex_destroy(&globals->mutex_stop);
			free_global_var(globals);
			return (0);
		}
	}
	return (1);
}

int	join_monitor(t_global *global_var)
{
	if (pthread_join(global_var->monitor->thread, NULL))
	{
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
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
