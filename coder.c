/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 20:54:35 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/30 03:44:55 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	should_finish(t_coder *coder)
{
	pthread_mutex_lock(&coder->globals->mutex_stop);
	if (coder->globals->stop)
	{
		pthread_mutex_unlock(&coder->globals->mutex_stop);
		return (1);
	}
	pthread_mutex_unlock(&coder->globals->mutex_stop);
	return (0);
}

static void	sleep_odd_ids(int id)
{
	if (id % 2)
		usleep(200);
}

static void	start_debugging(t_coder *coder)
{
	print_action(coder, "is debugging");
	precise_sleep(coder->globals->time_to_debug);
}

static void	start_refactoring(t_coder *coder)
{
	print_action(coder, "is refactoring");
	precise_sleep(coder->globals->time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;
	if (coder->globals->number_of_compiles_required == 0)
		return (NULL);
	sleep_odd_ids(coder->id);
	get_last_compile_time(coder);
	while (1)
	{
		insert_coder_to_heap(coder->globals->heap, coder);
		if (should_finish(coder))
			return (NULL);
		if (!waiting_to_compile(coder))
			return (NULL);
		start_compiling(coder);
		get_last_compile_time(coder);
		release_dongles(coder);
		increment_total_compiling(coder);
		if (should_finish(coder))
			return (NULL);
		start_debugging(coder);
		if (should_finish(coder))
			return (NULL);
		start_refactoring(coder);
	}
}
