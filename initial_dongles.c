/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:59:09 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/01 05:23:24 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*initial_dongles(int number_of_coders)
{
	t_dongle	*dongles;
	int			i;

	dongles = ft_memalloc(sizeof(t_dongle) * (number_of_coders));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < number_of_coders)
	{
		dongles[i].id = i + 1;
		dongles[i].is_taken = 0;
		if (pthread_mutex_init(&dongles[i].mutex_dongle, NULL))
		{
			destroy_mutexes_of_dongles_at(dongles, i);
			return (NULL);
		}
		i += 1;
	}
	return (dongles);
}
