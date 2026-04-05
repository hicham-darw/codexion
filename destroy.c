/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:56:29 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 01:42:44 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_mutexes_of_dongles_at(t_dongle *dongles, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&dongles[i].mutex_dongle);
	free(dongles);
	return ;
}

void	destroy_mutexes_of_coders_at(t_coder *coders, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&coders[i].mutex_coder);
}

void	destroy_conds_of_coders_at(t_coder *coders, int i)
{
	while (--i >= 0)
		pthread_cond_destroy(&coders[i].cond_coder);
	free(coders);
}
