/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:01:56 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/01 05:45:56 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_global_var(t_global *global_var)
{
	if (!global_var)
		return ;
	if (global_var->scheduler)
		free(global_var->scheduler);
	free_coders(global_var->coders, global_var->number_of_coders);
	free_dongles(global_var->dongles, global_var->number_of_coders);
	free_heap(global_var->heap);
	free_manager(global_var->manager);
	free_monitor(global_var->monitor);
}

void	free_dongles(t_dongle *dongles, int number_of_coders)
{
	int	i;

	if (!dongles || !number_of_coders)
		return ;
	i = 0;
	while (i < number_of_coders)
		pthread_mutex_destroy(&dongles[i++].mutex_dongle);
	free(dongles);
	dongles = NULL;
}

void	free_coders(t_coder *coders, int number_of_coders)
{
	int	i;

	if (!coders || !number_of_coders)
		return ;
	i = 0;
	while (i < number_of_coders)
	{
		pthread_mutex_destroy(&coders[i].mutex_coder);
		pthread_cond_destroy(&coders[i].cond_coder);
		i += 1;
	}
	free(coders);
}

void	free_monitor(t_monitor *monitor)
{
	if (!monitor)
		return ;
	free(monitor->coders);
	monitor->coders = NULL;
	free(monitor);
	monitor = NULL;
}

void	free_manager(t_manager *manager)
{
	if (!manager)
		return ;
	free(manager->dongles);
	manager->dongles = NULL;
	free(manager);
	manager = NULL;
}
