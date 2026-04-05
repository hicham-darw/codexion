/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:00:14 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 08:30:31 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	pop_coder_to_compile(t_manager *manager, t_coder *coder, int index)
{
	coder = pop_heap_at(manager->heap, index);
	pthread_mutex_lock(&coder->mutex_coder);
	coder->can_compile = 1;
	pthread_cond_broadcast(&coder->cond_coder);
	pthread_mutex_unlock(&coder->mutex_coder);
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
}

void	pop_all_available_to_compile(t_manager *manager)
{
	t_coder	*coder;
	int		i;

	i = 0;
	while (i < manager->heap->size)
	{
		coder = manager->heap->coders[i];
		if (take_dongles(coder))
		{
			print_action(coder, "has taken a dongle");
			print_action(coder, "has taken a dongle");
			pop_coder_to_compile(manager, coder, i);
			continue ;
		}
		i++;
	}
}

int	send_to_stop_coders(t_manager *manager)
{
	t_coder	*coder;

	pthread_mutex_lock(&manager->globals->mutex_stop);
	if (manager->globals->stop)
	{
		pthread_mutex_unlock(&manager->globals->mutex_stop);
		pthread_mutex_lock(&manager->heap->mutex_heap);
		while (!is_empty_heap(manager->heap))
		{
			coder = pop_heap_at(manager->heap, 0);
			pthread_mutex_lock(&coder->mutex_coder);
			coder->can_compile = 1;
			pthread_cond_broadcast(&coder->cond_coder);
			pthread_mutex_unlock(&coder->mutex_coder);
		}
		pthread_mutex_unlock(&manager->heap->mutex_heap);
		return (1);
	}
	pthread_mutex_unlock(&manager->globals->mutex_stop);
	return (0);
}

void	*manager_routine(void *arg)
{
	t_manager	*manager;

	manager = (t_manager *)arg;
	while (1)
	{
		if (send_to_stop_coders(manager))
			return (NULL);
		pthread_mutex_lock(&manager->heap->mutex_heap);
		if (is_empty_heap(manager->heap))
		{
			pthread_mutex_unlock(&manager->heap->mutex_heap);
			usleep(200);
			continue ;
		}
		pop_all_available_to_compile(manager);
		pthread_mutex_unlock(&manager->heap->mutex_heap);
		usleep(100);
	}
	return (NULL);
}
