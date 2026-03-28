#include "codexion.h"

t_manager	*initial_manager(t_global *global)
{
	t_manager	*manager;
	int			i;

	manager = (t_manager *)ft_memalloc(sizeof(t_manager));
	if (!manager)
		return (NULL);
	manager->dongles = (t_dongle **)ft_memalloc(sizeof(t_dongle *)
			* global->number_of_coders);
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
	manager->heap = global->heap;
	manager->globals = global;
	return (manager);
}

void	pop_coder_to_compile(t_manager *manager, t_coder *coder, int index)
{
	coder = pop_heap_at(manager->heap, index);
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
		{
			pop_coder_to_compile(manager, coder, i);
			continue ;
		}
		i++;
	}
}

int	send_to_stop_coders(t_manager *manager)
{
	t_coder	*coder;

	pthread_mutex_lock(&manager->globals->mutex_stop);
	if (manager->globals->stop)
	{
		pthread_mutex_unlock(&manager->globals->mutex_stop);
		pthread_mutex_lock(&manager->heap->mutex_heap);
		while (!is_empty_heap(manager->heap))
		{
			coder = pop_heap_at(manager->heap, 0);
			pthread_mutex_lock(&coder->mutex_coder);
			coder->can_compile = 1;
			pthread_cond_broadcast(&coder->cond_coder);
			pthread_mutex_unlock(&coder->mutex_coder);
		}
		pthread_mutex_unlock(&manager->heap->mutex_heap);
		return (1);
	}
	pthread_mutex_unlock(&manager->globals->mutex_stop);
	return (0);
}

void	*manager_routine(void *arg)
{
	t_manager	*manager;

	manager = (t_manager *)arg;
	while (1)
	{
		if (send_to_stop_coders(manager))
			return (NULL);
		pthread_mutex_lock(&manager->heap->mutex_heap);
		if (is_empty_heap(manager->heap))
		{
			pthread_mutex_unlock(&manager->heap->mutex_heap);
			usleep(200);
			continue ;
		}
		pop_all_available_to_compile(manager);
		pthread_mutex_unlock(&manager->heap->mutex_heap);
		usleep(100);
	}
	return (NULL);
}
