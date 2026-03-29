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
	heap->coders[heap->size] = coder;
	heapify_up(heap, heap->size);
	heap->size++;
	pthread_mutex_unlock(&heap->mutex_heap);
}

void	heapify_down(t_heap *heap, int index)
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
		if (
			left < heap->size
			&& coders[left]->last_compile < coders[smallest]->last_compile
		)
			smallest = left;
		if (
			right < heap->size
			&& coders[right]->last_compile < coders[smallest]->last_compile
		)
			smallest = right;
		if (smallest == index)
			break ;
		swap_coders(&heap->coders[index], &heap->coders[smallest]);
		index = smallest;
	}
}

t_coder	*pop_heap_at(t_heap *heap, int index)
{
	t_coder		*removed;

	if (!heap || heap->size <= 0 || index >= heap->size)
		return (NULL);
	removed = heap->coders[index];
	heap->coders[index] = heap->coders[heap->size - 1];
	heap->size--;
	heapify_down(heap, index);
	return (removed);
}

void	heapify_up(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->coders[parent]->id <= heap->coders[index]->id)
			break ;
		swap_coders(&heap->coders[parent], &heap->coders[index]);
		index = parent;
	}
}
