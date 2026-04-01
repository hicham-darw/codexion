/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_heap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:14:56 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/01 05:29:29 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_empty_heap(t_heap *heap)
{
	return (!heap || !heap->size);
}

int	is_in_heap(t_heap *heap, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->coders[i]->id == coder->id)
			return (1);
		i++;
	}
	return (0);
}

t_heap	*initial_heap(t_global **g)
{
	t_heap	*heap;

	heap = ft_memalloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->coders = ft_memalloc(sizeof(t_coder *) * (*g)->number_of_coders);
	if (!heap->coders)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	heap->capacity = (*g)->number_of_coders;
	if (pthread_mutex_init(&heap->mutex_heap, NULL))
	{
		free(heap->coders);
		free(heap);
		return (NULL);
	}
	return (heap);
}
