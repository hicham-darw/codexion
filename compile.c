/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:55:24 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/30 02:12:29 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	increment_total_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->total_compiling += 1;
	pthread_mutex_unlock(&coder->mutex_coder);
}

void	get_last_compile_time(t_coder *coder)
{
	int	time_burnout;

	pthread_mutex_lock(&coder->mutex_coder);
	coder->last_compile = get_time_by_milisecond();
	time_burnout = coder->globals->time_to_burnout;
	coder->deadline = coder->last_compile + time_burnout;
	pthread_mutex_unlock(&coder->mutex_coder);
}

int	waiting_to_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	while (!coder->can_compile)
	{
		pthread_mutex_lock(&coder->globals->mutex_stop);
		if (coder->globals->stop)
		{
			pthread_mutex_unlock(&coder->globals->mutex_stop);
			pthread_mutex_unlock(&coder->mutex_coder);
			return (0);
		}
		pthread_mutex_unlock(&coder->globals->mutex_stop);
		pthread_cond_wait(&coder->cond_coder, &coder->mutex_coder);
	}
	coder->can_compile = 0;
	pthread_mutex_unlock(&coder->mutex_coder);
	if (should_finish(coder))
		return (0);
	return (1);
}

void	start_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->is_compiling = 1;
	pthread_mutex_unlock(&coder->mutex_coder);
	print_action(coder, "is compiling");
	precise_sleep(coder->globals->time_to_compile);
	pthread_mutex_lock(&coder->mutex_coder);
	coder->is_compiling = 0;
	pthread_mutex_unlock(&coder->mutex_coder);
}
