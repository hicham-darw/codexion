/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 01:13:19 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/28 20:28:26 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_global	*global_var;

	if (ac != 9)
	{
		fprintf(stderr, USAGE1);
		return (42);
	}
	global_var = parse_arguments(av + 1);
	if (!global_var)
		return (42);
	if (
		!initializer(&global_var)
		|| !create_threads(global_var)
		|| !join_threads(global_var)
	)
		return (42);
	pthread_mutex_destroy(&global_var->mutex_print);
	pthread_mutex_destroy(&global_var->mutex_time);
	pthread_mutex_destroy(&global_var->mutex_stop);
	free_global_var(global_var);
	return (0);
}
