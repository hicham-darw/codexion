#include "codexion.h"


int		is_empty_heap(t_heap *heap)
{
    return (!heap || !heap->size);
}

int     is_in_heap(t_heap *heap, t_coder *coder)
{
    int     i;

    i = 0;
    while (i < heap->size)
    {
        if (heap->coders[i]->id == coder->id)
            return 1;
        i++;
    }
    return 0;
}

void swap_coders(t_coder **a, t_coder **b)
{
    t_coder *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

t_heap  *initial_heap(t_global **global_var)
{
    t_heap  *heap;
    int     i;

    if (!(*global_var))
        return (NULL);
    heap = malloc(sizeof(t_heap));
    if (!heap)
        return (0);
    heap->coders = (t_coder **)malloc(sizeof(t_coder *) * (*global_var)->number_of_coders);
    if (!heap->coders)
    {
        free(heap);
        return (0);
    }

    i = 0;
    while (i < (*global_var)->number_of_coders)
    {
        heap->coders[i] = &(*global_var)->coders[i];
        i += 1;
    }

    heap->size = 0;
    heap->capacity = (*global_var)->number_of_coders;
    if (pthread_mutex_init(&heap->mutex_heap, NULL))
    {
        free(heap->coders);
        free(heap);
        return (NULL);
    }
    return (heap);
}


void insert_coder_to_heap(t_heap *heap, t_coder *coder)
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

void heapify_down(t_heap *heap, int index)
{
    int left;
    int right;
    int smallest;

    while (1)
    {
        left = index * 2 + 1;
        right = index * 2 + 2;
        smallest = index;

        if (left < heap->size &&
            heap->coders[left]->last_compile_time < heap->coders[smallest]->last_compile_time)
            smallest = left;

        if (right < heap->size &&
            heap->coders[right]->last_compile_time < heap->coders[smallest]->last_compile_time)
            smallest = right;

        if (smallest == index)
            break;

        swap_coders(&heap->coders[index], &heap->coders[smallest]);
        index = smallest;
    }
}

t_coder *pop_heap_at(t_heap *heap, int index)
{
    if (!heap || heap->size <= 0 || index >= heap->size)
        return NULL;

    t_coder *removed = heap->coders[index];

    // Move last element to index
    heap->coders[index] = heap->coders[heap->size - 1];
    heap->size--;

    // Heapify down from the index
    heapify_down(heap, index);

    return removed;
}


void heapify_up(t_heap *heap, int index)
{
    int parent;

    while (index > 0)
    {
        parent = (index - 1) / 2;

        if (heap->coders[parent]->id <= heap->coders[index]->id)
            break;

        swap_coders(&heap->coders[parent], &heap->coders[index]);
        index = parent;
    }
}

