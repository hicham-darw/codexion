/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:00:26 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/31 23:53:24 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	add_parsed_number(t_global **global, int number, int index)
{
	if (!global)
		return (0);
	if (index == 0)
		(*global)->number_of_coders = number;
	else if (index == 1)
		(*global)->time_to_burnout = number;
	else if (index == 2)
		(*global)->time_to_compile = number;
	else if (index == 3)
		(*global)->time_to_debug = number;
	else if (index == 4)
		(*global)->time_to_refactor = number;
	else if (index == 5)
		(*global)->number_of_compiles_required = number;
	else if (index == 6)
		(*global)->dongle_cooldown = number;
	return (1);
}

static t_global	*parse_numbers(t_global *valid_input, char **av)
{
	long long	p_num;
	int			i;

	i = 0;
	while (av[i] && i < 7)
	{
		p_num = ft_atoll(av[i]);
		if (p_num < 0 || (p_num == 0 && i == 0) || p_num > INT_MAX)
		{
			free_global_var(valid_input);
			if (i == 0)
				fprintf(stderr, ERR_NUMBER_OF_CODERS);
			else
				fprintf(stderr, ERR_INVALID_ARG);
			return (NULL);
		}
		if (add_parsed_number(&valid_input, p_num, i) <= 0)
		{
			free(valid_input);
			return (NULL);
		}
		i += 1;
	}
	return (valid_input);
}

static char	*parse_schedular(char *arg)
{
	char	*scheduler;
	int		cmp_1;
	int		cmp_2;

	scheduler = ft_strtrim(arg);
	if (!scheduler)
		return (NULL);
	cmp_1 = ft_strcmp(ft_strtolower(scheduler), "fifo");
	cmp_2 = ft_strcmp(ft_strtolower(scheduler), "edf");
	if (cmp_1 && cmp_2)
	{
		free(scheduler);
		scheduler = NULL;
		return (NULL);
	}
	return (ft_strtolower(scheduler));
}

t_global	*parse_arguments(char **av)
{
	t_global	*valid_input;

	if (!av || !*av)
		return (NULL);
	valid_input = malloc(sizeof(t_global));
	if (!valid_input)
		return (NULL);
	memset(valid_input, 0, sizeof(t_global));
	valid_input = parse_numbers(valid_input, av);
	if (!valid_input)
		return (NULL);
	valid_input->scheduler = parse_schedular(av[7]);
	if (!valid_input->scheduler)
	{
		free(valid_input);
		return (NULL);
	}
	return (valid_input);
}
