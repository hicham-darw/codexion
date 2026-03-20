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
		return (NULL);

	i = 0;
	while (i < global->number_of_coders)
	{
		manager->dongles[i] = &(global->dongles[i]);
		i += 1;
	}
	manager->heap = malloc(sizeof(t_heap));
	if (!manager->heap)
		return (NULL);
	manager->heap->coders = (t_coder **)malloc(sizeof(t_coder *) * global->number_of_coders);
	if (!manager->heap->coders)
		return (NULL);
	manager->heap->size = 0;
	manager->heap->capacity = global->number_of_coders;
	manager->globals = global;
	pthread_mutex_init(&manager->heap->mutex_heap, NULL);
	return (manager);
}

int	take_dongles(t_coder * coder)
{
	int		ret_val;

	if (coder->id % 2)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
		pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
	}
	if (!coder->left_dongle->is_taken && !coder->right_dongle->is_taken)
	{
		coder->left_dongle->is_taken = 1;
		coder->right_dongle->is_taken = 1;
		return (1);
	}
	ret_val = 0;
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
	return ret_val;
}

void	pop_coder_to_compile(t_manager *manager, t_coder *coder, int index_in_heap)
{
	coder = pop_heap_at(manager->heap, index_in_heap); // remove coder from heap
	pthread_mutex_lock(&coder->mutex_coder);
	coder->can_compile = 1;
	pthread_cond_broadcast(&coder->cond_coder);
	pthread_mutex_unlock(&coder->mutex_coder);
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
}

void	pop_all_available_to_compile(t_manager *manager)
{
    t_coder	*coder;
	int		i;

    i = 0;
    while (i < manager->heap->size)
    {
        coder = manager->heap->coders[i];
		if (take_dongles(coder))
			pop_coder_to_compile(manager, coder, i);
		i++;
		pthread_mutex_unlock(&manager->heap->mutex_heap);
   	}
}

void *manager_routine(void *arg)
{
    t_manager *manager;
    int i;

	manager = (t_manager *)arg;
	while (/*!manager->globals->is_finished*/1)
    {
		pthread_mutex_lock(&manager->heap->mutex_heap);
        if (is_empty_heap(manager->heap))
		{
			pthread_mutex_unlock(&manager->heap->mutex_heap);
			usleep(500);
			continue;
		}
		pop_all_available_to_compile(manager);
	}
    return NULL;
}