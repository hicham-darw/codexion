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
	pthread_cond_init(&manager->heap->cond_heap, NULL);
	return (manager);
}

int		is_empty_heap(t_heap *heap, int number_of_coders)
{
	int		ret_val;

	pthread_mutex_lock(&heap->mutex_heap);
	if (!heap || !heap->size)
		ret_val = 1;
	else
		ret_val = 0;
	pthread_mutex_unlock(&heap->mutex_heap);
	return ret_val;
}


void wake_coder(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);

    coder->both_available = 1;
	coder->left_dongle->is_taken = 1;
	coder->right_dongle->is_taken = 1;
    pthread_cond_signal(&coder->cond_coder);

    pthread_mutex_unlock(&coder->mutex_coder);
}

int	is_dongles_available(t_coder * coder)
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
		ret_val = 1;
	}
	else
	{
		ret_val = 0;
		pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
		pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
	}

	return ret_val;
}


void *manager_routine(void *arg)
{
    t_manager *manager = (t_manager *)arg;
    t_coder *coder;
    int i;

	while (/*!manager->globals->is_finished*/1)
    {
        while (is_empty_heap(manager->heap, manager->globals->number_of_coders))
			usleep(500);
		pthread_mutex_lock(&manager->heap->mutex_heap);
        i = 0;
        while (i < manager->heap->size)
        {
			// printf("now heap is locked by manager\n");
            coder = manager->heap->coders[i];
			if (is_dongles_available(coder))
            {
				pthread_mutex_lock(&coder->mutex_coder);
				coder->can_compile = 1;
				pthread_cond_signal(&coder->cond_coder);
				pthread_mutex_unlock(&coder->mutex_coder);
			}
			i++;
		}
		pthread_mutex_unlock(&manager->heap->mutex_heap);
		usleep(500);
	}
    return NULL;
}