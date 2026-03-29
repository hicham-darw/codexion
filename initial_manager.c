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