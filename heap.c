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

void	swap_coders(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_heap	*initial_heap(t_global **g)
{
	t_heap	*heap;
	int		i;

	if (!(*g))
		return (NULL);
	heap = ft_memalloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->coders = ft_memalloc(sizeof(t_coder *) * (*g)->number_of_coders);
	if (!heap->coders)
	{
		free(heap);
		return (NULL);
	}
	i = 0;
	while (i < (*g)->number_of_coders)
	{
		heap->coders[i] = &(*g)->coders[i];
		i += 1;
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
