/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:10:24 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 08:37:03 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	insert_coder_to_heap(t_heap *heap, t_coder *coder)
{
	pthread_mutex_lock(&heap->mutex_heap);
	if (!heap || !coder || heap->size >= heap->capacity)
	{
		pthread_mutex_unlock(&heap->mutex_heap);
		return ;
	}
	if (is_in_heap(heap, coder))
	{
		pthread_mutex_unlock(&heap->mutex_heap);
		return ;
	}
	coder->arrival = get_time_by_milisecond();
	heap->coders[heap->size] = coder;
	if (!ft_strcmp(coder->globals->scheduler, EDF))
		heapify_up_by_edf(heap, heap->size);
	else
		heapify_up_by_fifo(heap, heap->size);
	heap->size++;
	pthread_mutex_unlock(&heap->mutex_heap);
}

t_coder	*pop_heap_at(t_heap *heap, int index)
{
	t_coder		*removed;

	if (!heap || heap->size <= 0 || index >= heap->size)
		return (NULL);
	removed = heap->coders[index];
	heap->coders[index] = heap->coders[heap->size - 1];
	heap->size--;
	if (!ft_strcmp(removed->globals->scheduler, EDF))
		heapify_down_by_edf(heap, index);
	else
		heapify_down_by_fifo(heap, index);
	return (removed);
}

void	free_heap(t_heap *heap)
{
	if (!heap)
		return ;
	free(heap->coders);
	heap->coders = NULL;
	pthread_mutex_destroy(&heap->mutex_heap);
	free(heap);
	heap = NULL;
}
