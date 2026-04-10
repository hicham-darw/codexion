/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:24:11 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/10 21:44:55 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	change_stop_var(t_monitor *monitor)
{
	pthread_mutex_lock(&monitor->globals->mutex_stop);
	monitor->globals->stop = 1;
	pthread_mutex_unlock(&monitor->globals->mutex_stop);
}

void	check_each_coder(t_coder *coder, int *finish_compile, int *burnout)
{
	pthread_mutex_lock(&coder->mutex_coder);
	if (
		coder->globals->number_of_coders == 1
		&& coder->last_compile
		&& burnout_coder(coder)
	)
	{
		*burnout = 1;
		pthread_mutex_unlock(&coder->mutex_coder);
		return ;
	}
	if (coder->globals->number_of_compiles_required <= coder->total_compiling)
		*finish_compile += 1;
	if (
		!coder->is_compiling
		&& burnout_coder(coder)
		&& coder->last_compile
	)
	{
		*burnout = 1;
		pthread_mutex_unlock(&coder->mutex_coder);
		return ;
	}
	pthread_mutex_unlock(&coder->mutex_coder);
}

static int	should_stop(t_monitor *monitor, int i, int burnout, int n_compiles)
{
	if (burnout)
	{
		print_action(monitor->coders[i], "burned out");
		return (1);
	}
	if (n_compiles >= monitor->globals->number_of_coders)
		return (1);
	return (0);
}

int	error_in_time_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	if (
		coder->last_compile == -1
		|| coder->arrival == -1
		|| coder->deadline == -1
	)
	{
		pthread_mutex_unlock(&coder->mutex_coder);
		change_stop_var(coder->globals->monitor);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex_coder);
	return (0);
}

void	*monitor_routine(void *args)
{
	t_monitor	*monitor;
	int			finished;
	int			i;
	int			burned_out;

	monitor = (t_monitor *)args;
	burned_out = 0;
	while (1)
	{
		i = -1;
		finished = 0;
		while (++i < monitor->globals->number_of_coders)
		{
			if (error_in_time_coder(monitor->coders[i]))
				return (NULL);
			check_each_coder(monitor->coders[i], &finished, &burned_out);
			if (burned_out)
				break ;
		}
		if (should_stop(monitor, i, burned_out, finished))
			break ;
		usleep(500);
	}
	change_stop_var(monitor);
	return (NULL);
}
