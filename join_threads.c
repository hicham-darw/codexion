/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:58:41 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 03:09:00 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	join_manager(t_global *global_var)
{
	int		i;

	if (pthread_join(global_var->manager->thread, NULL))
	{
		i = -1;
		while (++i < global_var->number_of_coders)
			pthread_join(global_var->coders[i].thread, NULL);
		pthread_join(global_var->monitor->thread, NULL);
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		pthread_mutex_destroy(&global_var->mutex_stop);
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	return (TRUE);
}

int	join_coders_at(t_coder *coders, int number_of_coders)
{
	t_global	*globals;
	int			i;

	globals = coders[0].globals;
	i = -1;
	while (++i < number_of_coders)
	{
		if (pthread_join(coders[i].thread, NULL))
		{
			while (++i < number_of_coders)
				pthread_join(coders[i].thread, NULL);
			pthread_join(globals->monitor->thread, NULL);
			pthread_mutex_destroy(&globals->mutex_time);
			pthread_mutex_destroy(&globals->mutex_stop);
			pthread_mutex_destroy(&globals->mutex_print);
			free_global_var(globals);
			free(globals);
			return (FALSE);
		}
	}
	return (TRUE);
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
		return (FALSE);
	}
	return (TRUE);
}

int	join_threads(t_global *global_var)
{
	if (!join_manager(global_var))
		return (FALSE);
	if (!join_coders_at(global_var->coders, global_var->number_of_coders))
		return (FALSE);
	if (!join_monitor(global_var))
		return (FALSE);
	return (TRUE);
}
