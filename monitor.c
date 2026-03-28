#include "codexion.h"


static void	change_stop_var(t_monitor *monitor)
{
	pthread_mutex_lock(&monitor->globals->mutex_stop);
	monitor->globals->stop = 1;
	pthread_mutex_unlock(&monitor->globals->mutex_stop);
}

void	check_each_coder(t_coder *coder, int *finish_compile, int *burnout)
{
	pthread_mutex_lock(&coder->mutex_coder);
	if (
		coder->globals->number_of_coders == 1
		&& coder->last_compile
		&& burnout_coder(coder)
	)
	{
		*burnout = 1;
		pthread_mutex_unlock(&coder->mutex_coder);
		return ;
	}
	if (coder->globals->number_of_compiles_required == coder->total_compiling)
		finish_compile += 1;
	if (
		!coder->is_compiling
		&& burnout_coder(coder)
		&& (coder->last_compile != 0)
	)
	{
		*burnout = 1;
		pthread_mutex_unlock(&coder->mutex_coder);
		return ;
	}
	pthread_mutex_unlock(&coder->mutex_coder);
}

static int	should_stop(t_monitor *monitor, int i, int burnout, int n_compiles)
{
	if (burnout)
	{
		print_log(monitor->coders[i], "is burned out!!!!!");
		return (1);
	}
	if (n_compiles == monitor->globals->number_of_coders)
		return (1);
	return (0);
}

void	*monitor_routine(void *args)
{
	t_monitor	*monitor;
	int			finished;
	int			i;
	int			burned_out;

	monitor = (t_monitor *)args;
	burned_out = 0;
	while (1)
	{
		i = 0;
		finished = 0;
		while (i < monitor->globals->number_of_coders)
		{
			check_each_coder(monitor->coders[i], &finished, &burned_out);
			if (burned_out)
				break ;
			i++;
		}
		if (should_stop(monitor, i, burned_out, finished))
			break ;
		usleep(500);
	}
	change_stop_var(monitor);
	return (NULL);
}
