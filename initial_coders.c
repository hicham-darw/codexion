#include "codexion.h"


t_coder	*initial_coders(t_global **global_var)
{
	t_coder *coders;
	int	i;

	if (!global_var)
		return (NULL);
	i = 0;
	coders = malloc(sizeof(t_coder) * ((*global_var)->number_of_coders));
	if (!coders)
		return (NULL);
	while (i < (*global_var)->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].is_compiling = 0;
		coders[i].is_debugging = 0;
		coders[i].is_refactoring = 0;
		coders[i].left_dongle = &((*global_var)->dongles[i]);
		if (i < (*global_var)->number_of_coders - 1)
			coders[i].right_dongle = &((*global_var)->dongles[i + 1]);
		else
			coders[i].right_dongle = &((*global_var)->dongles[0]);
		i ++;
	}
	return (coders);
}
