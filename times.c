#include "codexion.h"

time_t	get_time_by_milisecond(void)
{
	struct timeval		t_val;

	gettimeofday(&t_val, NULL);
	return ((t_val.tv_sec * 1000) + (t_val.tv_usec / 1000));
}

int	burnout_coder(t_coder *coder)
{
	time_t	now;

	now = get_time_by_milisecond();
	if (now - coder->last_compile > coder->globals->time_to_burnout)
		return (1);
	return (0);
}

void	precise_sleep(long time_ms)
{
	long	start;

	start = get_time_by_milisecond();
	while (get_time_by_milisecond() - start < time_ms)
		usleep(200);
}

void	print_action(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->globals->mutex_print);
	printf(
		"%ld %d %s\n",
		get_time_by_milisecond() - coder->globals->start_time,
		coder->id,
		msg);
	pthread_mutex_unlock(&coder->globals->mutex_print);
}
