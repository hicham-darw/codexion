#include "codexion.h"



t_manager	*initial_manager(t_global *global)
{
	t_manager *manager;
	int		i;

	manager = (t_manager *)malloc(sizeof(t_manager));
	if (!manager)
		return (NULL);
	manager->dongles = (t_dongle **)malloc(sizeof(t_dongle *) * global->number_of_coders);
	if (!manager->dongles)
	{
		free(manager);
		return (NULL);
	}
	i = 0;
	while (i < global->number_of_coders)
	{
		manager->dongles[i] = &(global->dongles[i]);
		i += 1;
	}
	manager->heap = malloc(sizeof(t_heap));
	if (!manager->heap)
	{
		free(manager);
		free(manager->dongles);
		return (NULL);
	}
	manager->heap->coders = (t_coder **)malloc(sizeof(t_coder *) * global->number_of_coders);
	if (!manager->heap->coders)
	{
		free(manager);
		free(manager->dongles);
		free(manager->heap);
		return (NULL);
	}
	if (!strcmp(global->schedular, "fifo") || !strcmp(global->schedular, "FIFO"))
		manager->heap->schedular = 1;
	else
		manager->heap->schedular = 0;
	manager->heap->new_elem = 0;
	manager->heap->size = 0;
	manager->heap->capacity = global->number_of_coders;
	manager->globals = global;
	pthread_mutex_init(&manager->heap->mutex_heap, NULL);
	pthread_cond_init(&manager->heap->cond_heap, NULL);
	return (manager);
}

int		is_empty_heap(t_heap *heap)
{
	int	is_empty;

	pthread_mutex_lock(&heap->mutex_heap);
	if (!heap->size)
		is_empty = 1;
	else
		is_empty = 0;
	pthread_mutex_unlock(&heap->mutex_heap);

	return is_empty;
}

t_coder *pop_heap(t_heap **heap)
{
	t_heap *head;
	t_coder	*coder;
	int		i;

	if (!heap)
		return (NULL);
	coder = (*heap)->coders[0];
	i = 1;
	while (i < (*heap)->size)
	{
		heap[i - 1] = heap[i]; 
		i += 1;
	}
	(*heap)->size -= 1;
	return coder;
}

void wake_coder(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);

    coder->both_available = 1;

    pthread_cond_signal(&coder->cond_coder);

    pthread_mutex_unlock(&coder->mutex_coder);
}

int	is_dongles_available(t_coder * coder)
{
	t_dongle *left;
	t_dongle *right;

	printf("here!1!\n");
	int		available = 0;
	if (coder->id % 2)
	{
		pthread_mutex_lock(&left->mutex_dongle);
		pthread_mutex_lock(&right->mutex_dongle);
	}
	else
	{
		pthread_mutex_lock(&right->mutex_dongle);
		pthread_mutex_lock(&left->mutex_dongle);
	}
	if (left->is_taken || right->is_taken)
	{
		pthread_mutex_unlock(&right->mutex_dongle);
		pthread_mutex_unlock(&left->mutex_dongle);
		return 0;
	}
	else
		return 1;
}

void *manager_routine(void *arg)
{
    t_manager *manager = (t_manager *)arg;
    t_coder *coder;

    while (!manager->globals->is_finished)
    {
        pthread_mutex_lock(&manager->heap->mutex_heap);

        while (is_empty_heap(manager->heap))
            pthread_cond_wait(&manager->heap->cond_heap, &manager->heap->mutex_heap);
		coder = manager->heap->coders[0];
		if (is_dongles_available(coder))
		{
			coder = pop_heap(&manager->heap);
			wake_coder(coder);
		}
		else
			coder = pop_heap(&manager->heap);
		/// continue here read about insert dletee element in heap how queue can give coders signals to wakeup and run independently!
		
        pthread_mutex_unlock(&manager->heap->mutex_heap);

        wake_coder(coder);
    }

    return NULL;
}