/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:58:50 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 03:13:11 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

time_t	get_time_by_milisecond(void)
{
	struct timeval		t_val;

	if (gettimeofday(&t_val, NULL))
	{
		fprintf(stderr, "Error: getimeofday failed!\n");
		return -1;
	}
	return ((t_val.tv_sec * 1000) + (t_val.tv_usec / 1000));
}

int	burnout_coder(t_coder *coder)
{
	time_t	now;

	now = get_time_by_milisecond();
	if (now - coder->last_compile > coder->globals->time_to_burnout)
		return (1);
	return (0);
}

void	precise_sleep(long time_ms)
{
	long	start;

	start = get_time_by_milisecond();
	while (get_time_by_milisecond() - start < time_ms)
		usleep(200);
}

void	print_action(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->globals->mutex_print);
	printf(
		"%ld %d %s\n",
		get_time_by_milisecond() - coder->globals->start_time,
		coder->id,
		msg);
	pthread_mutex_unlock(&coder->globals->mutex_print);
}
