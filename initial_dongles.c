/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:59:09 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/30 00:59:11 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*initial_dongles(int number_of_coders)
{
	t_dongle	*dongle;
	int			i;

	dongle = malloc(sizeof(t_dongle) * (number_of_coders));
	if (!dongle)
		return (NULL);
	memset(dongle, 0, sizeof(t_dongle) * (number_of_coders));
	i = 0;
	while (i < number_of_coders)
	{
		dongle[i].id = i + 1;
		dongle[i].is_taken = 0;
		if (pthread_mutex_init(&dongle[i].mutex_dongle, NULL))
		{
			destroy_mutexes_of_dongles_at(dongle, i);
			return (NULL);
		}
		i += 1;
	}
	return (dongle);
}
