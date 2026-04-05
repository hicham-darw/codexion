/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:20:58 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 02:16:03 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	initial_dongles_and_coders_and_heap(t_global **global_var)
{
	(*global_var)->dongles = initial_dongles((*global_var)->number_of_coders);
	if (!(*global_var)->dongles)
	{
		free((*global_var)->scheduler);
		free((*global_var));
		return (FALSE);
	}
	(*global_var)->coders = initial_coders(global_var);
	if (!(*global_var)->coders)
	{
		free_dongles((*global_var)->dongles, (*global_var)->number_of_coders);
		free((*global_var)->scheduler);
		free((*global_var));
		return (FALSE);
	}
	(*global_var)->heap = initial_heap(global_var);
	if (!(*global_var)->heap)
	{
		free_dongles((*global_var)->dongles, (*global_var)->number_of_coders);
		free_coders((*global_var)->coders, (*global_var)->number_of_coders);
		free((*global_var)->scheduler);
		free((*global_var));
		return (FALSE);
	}
	return (TRUE);
}

static int	initial_global_mutexes(t_global *global_var)
{
	if (pthread_mutex_init(&global_var->mutex_print, NULL))
	{
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	if (pthread_mutex_init(&global_var->mutex_time, NULL))
	{
		pthread_mutex_destroy(&global_var->mutex_print);
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	if (pthread_mutex_init(&global_var->mutex_stop, NULL))
	{
		pthread_mutex_destroy(&global_var->mutex_print);
		pthread_mutex_destroy(&global_var->mutex_time);
		free_global_var(global_var);
		free(global_var);
		return (FALSE);
	}
	return (TRUE);
}

int	initializer(t_global **global_var)
{
	if (!initial_dongles_and_coders_and_heap(global_var))
		return (FALSE);
	(*global_var)->manager = initial_manager((*global_var));
	if (!(*global_var)->manager)
	{
		free_global_var((*global_var));
		free(*global_var);
		return (FALSE);
	}
	(*global_var)->monitor = initial_monitor(*global_var);
	if (!(*global_var)->monitor)
	{
		free_global_var(*global_var);
		free(*global_var);
		return (FALSE);
	}
	if (!initial_global_mutexes(*global_var))
		return (FALSE);
	return (TRUE);
}
