/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heapify.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <hel-hamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 02:38:16 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/01 01:27:24 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heapify_down_by_fifo(t_heap *heap, int index)
{
	t_coder	**coders;
	int		left;
	int		right;
	int		smallest;

	coders = heap->coders;
	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		smallest = index;
		if (left < heap->size
			&& coders[left]->arrival < coders[smallest]->arrival)
			smallest = left;
		if (right < heap->size
			&& coders[right]->arrival < coders[smallest]->arrival)
			smallest = right;
		if (smallest == index)
			break ;
		swap_coders(&heap->coders[index], &heap->coders[smallest]);
		index = smallest;
	}
}

void	heapify_down_by_edf(t_heap *heap, int index)
{
	t_coder	**coders;
	int		left;
	int		right;
	int		smallest;

	coders = heap->coders;
	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		smallest = index;
		if (left < heap->size
			&& coders[left]->deadline < coders[smallest]->deadline)
			smallest = left;
		if (right < heap->size
			&& coders[right]->deadline < coders[smallest]->deadline)
			smallest = right;
		if (smallest == index)
			break ;
		swap_coders(&heap->coders[index], &heap->coders[smallest]);
		index = smallest;
	}
}

void	heapify_up_by_fifo(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->coders[parent]->arrival <= heap->coders[index]->arrival)
			break ;
		swap_coders(&heap->coders[parent], &heap->coders[index]);
		index = parent;
	}
}

void	heapify_up_by_edf(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->coders[parent]->deadline <= heap->coders[index]->deadline)
			break ;
		swap_coders(&heap->coders[parent], &heap->coders[index]);
		index = parent;
	}
}
