/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 20:54:35 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/28 01:07:43 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"codexion.h"

static int	should_stop(t_coder *coder)
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

int	initial_mutex_coder(t_coder *coders, int i)
{
	if (pthread_mutex_init(&coders[i].mutex_coder, NULL))
	{
		destroy_mutexes_of_coders_at(coders, i - 1);
		destroy_conds_of_coders_at(coders, i - 1);
		return (0);
	}
	return (1);
}

int	initial_cond_coder(t_coder *coders, int i)
{
	if (pthread_cond_init(&coders[i].cond_coder, NULL))
	{
		destroy_mutexes_of_coders_at(coders, i);
		destroy_conds_of_coders_at(coders, i - 1);
		return (0);
	}
	return (1);
}

t_coder	*initial_coders(t_global **global_var)
{
	t_coder		*coders;
	int			i;

	if (!global_var)
		return (NULL);
	i = 0;
	coders = ft_memalloc(sizeof(t_coder) * ((*global_var)->number_of_coders));
	if (!coders)
		return (NULL);
	while (i < (*global_var)->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].left_dongle = &((*global_var)->dongles[i]);
		if (i < (*global_var)->number_of_coders - 1)
			coders[i].right_dongle = &((*global_var)->dongles[i + 1]);
		else
			coders[i].right_dongle = &((*global_var)->dongles[0]);
		coders[i].globals = (*global_var);
		if (!initial_mutex_coder(coders, i))
			return (NULL);
		if (!initial_cond_coder(coders, i))
			return (NULL);
		i ++;
	}
	return (coders);
}

static int	waiting_to_compile(t_coder *coder)
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
	if (should_stop(coder))
		return (0);
	return (1);
}

static void	start_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->is_compiling = 1;
	pthread_mutex_unlock(&coder->mutex_coder);
	print_log(coder, "is compiling");
	precise_sleep(coder->globals->time_to_compile);
	pthread_mutex_lock(&coder->mutex_coder);
	coder->is_compiling = 0;
	pthread_mutex_unlock(&coder->mutex_coder);
}

static void	start_debugging(t_coder *coder)
{
	print_log(coder, "is debugging");
	precise_sleep(coder->globals->time_to_debug);
}

static void	start_refactoring(t_coder *coder)
{
	print_log(coder, "is refactoring");
	precise_sleep(coder->globals->time_to_refactor);
}

static void	increment_total_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->total_compiling += 1;
	pthread_mutex_unlock(&coder->mutex_coder);
}

static void	get_last_compile_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->last_compile_time = get_time_by_milisecond();
	pthread_mutex_unlock(&coder->mutex_coder);
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
		if (should_stop(coder))
			return (NULL);
		if (!waiting_to_compile(coder))
			return (NULL);
		start_compiling(coder);
		get_last_compile_time(coder);
		release_dongles(coder);
		increment_total_compiling(coder);
		if (should_stop(coder))
			return (NULL);
		start_debugging(coder);
		if (should_stop(coder))
			return (NULL);
		start_refactoring(coder);
	}
}
