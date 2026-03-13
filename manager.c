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
	pthread_mutex_init(&manager->heap->mutex_heap, NULL);
	pthread_cond_init(&manager->heap->cond_heap, NULL);
	return (manager);
}
