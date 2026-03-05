#include "codexion.h"

time_t	get_time_by_milisecond(void)
{
	struct timeval		t_val;

	gettimeofday(&t_val, NULL);
	return ((t_val.tv_sec * 1000) + (t_val.tv_usec / 1000));
}
