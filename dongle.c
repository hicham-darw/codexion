/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:00:43 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/30 01:04:13 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	lock_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
		return (0);
	if (coder->id % 2)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
		pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
	}
	return (1);
}

int	take_dongles(t_coder *coder)
{
	time_t	now;

	if (!lock_dongles(coder))
		return (0);
	now = get_time_by_milisecond();
	if (
		!coder->left_dongle->is_taken
		&& !coder->right_dongle->is_taken
		&& coder->left_dongle->cooldown_time <= now
		&& coder->right_dongle->cooldown_time <= now
	)
	{
		print_action(coder, "has taken a dongle");
		print_action(coder, "has taken a dongle");
		coder->left_dongle->is_taken = 1;
		coder->right_dongle->is_taken = 1;
		return (1);
	}
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
	return (0);
}

void	release_dongles(t_coder *coder)
{
	time_t	now;

	if (coder->left_dongle == coder->right_dongle)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
		coder->left_dongle->is_taken = 0;
		pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
		return ;
	}
	lock_dongles(coder);
	now = get_time_by_milisecond();
	coder->left_dongle->cooldown_time = now + coder->globals->dongle_cooldown;
	coder->right_dongle->cooldown_time = now + coder->globals->dongle_cooldown;
	coder->left_dongle->is_taken = 0;
	coder->right_dongle->is_taken = 0;
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
}
