#include "codexion.h"

int	initial_mutex_coder(t_coder *coders, int i)
{
	if (pthread_mutex_init(&coders[i].mutex_coder, NULL))
	{
		destroy_mutexes_of_coders_at(coders, i - 1);
		destroy_conds_of_coders_at(coders, i - 1);
		return (0);
	}
	return (1);
}

int	initial_cond_coder(t_coder *coders, int i)
{
	if (pthread_cond_init(&coders[i].cond_coder, NULL))
	{
		destroy_mutexes_of_coders_at(coders, i);
		destroy_conds_of_coders_at(coders, i - 1);
		return (0);
	}
	return (1);
}

t_coder	*initial_coders(t_global **global_var)
{
	t_coder		*coders;
	int			i;

	if (!global_var)
		return (NULL);
	i = 0;
	coders = ft_memalloc(sizeof(t_coder) * ((*global_var)->number_of_coders));
	if (!coders)
		return (NULL);
	while (i < (*global_var)->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].left_dongle = &((*global_var)->dongles[i]);
		if (i < (*global_var)->number_of_coders - 1)
			coders[i].right_dongle = &((*global_var)->dongles[i + 1]);
		else
			coders[i].right_dongle = &((*global_var)->dongles[0]);
		coders[i].globals = (*global_var);
		if (!initial_mutex_coder(coders, i))
			return (NULL);
		if (!initial_cond_coder(coders, i))
			return (NULL);
		i ++;
	}
	return (coders);
}

void	swap_coders(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}