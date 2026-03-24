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
	memset(coders, 0, sizeof(t_coder) * ((*global_var)->number_of_coders));
	while (i < (*global_var)->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].left_dongle = &((*global_var)->dongles[i]);
		coders[i].start_time = 0;
		if (i < (*global_var)->number_of_coders - 1)
			coders[i].right_dongle = &((*global_var)->dongles[i + 1]);
		else
			coders[i].right_dongle = &((*global_var)->dongles[0]);
		coders[i].globals = (*global_var);
		pthread_mutex_init(&coders[i].mutex_coder, NULL);
		pthread_cond_init(&coders[i].cond_coder, NULL);
		i ++;
	}
	return (coders);
}

void    waiting_to_compile(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    while (!coder->can_compile)
    {
        pthread_mutex_lock(&coder->globals->mutex_stop);
        if (coder->globals->stop)
        {
            pthread_mutex_unlock(&coder->globals->mutex_stop);
            pthread_mutex_unlock(&coder->mutex_coder);
            return ;
        }
        pthread_mutex_unlock(&coder->globals->mutex_stop);
        pthread_cond_wait(&coder->cond_coder, &coder->mutex_coder);
    }
    coder->can_compile = 0;
    pthread_mutex_unlock(&coder->mutex_coder);
}

void    start_compiling(t_coder *coder)
{
    print_log(coder, "is compiling");        
    precise_sleep(coder->globals->time_to_compile);
}

void    start_debugging(t_coder *coder)
{
    print_log(coder, "is debugging");
    precise_sleep(coder->globals->time_to_debug);
}

void    start_refactoring(t_coder *coder)
{
    print_log(coder, "is refactoring");
    precise_sleep(coder->globals->time_to_refactor);
}

void    increment_total_compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    coder->total_compiling += 1;
    pthread_mutex_unlock(&coder->mutex_coder);
}

void    get_start_time_of_coders(t_coder *coder)
{
    pthread_mutex_lock(&coder->globals->mutex_time);
    if (!coder->globals->start_time)
        coder->globals->start_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->globals->mutex_time);
}


void    get_last_compile_time(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex_coder);
    coder->last_compile_time = get_time_by_milisecond();
    pthread_mutex_unlock(&coder->mutex_coder);
}

int    should_stop(t_coder * coder)
{
    pthread_mutex_lock(&coder->globals->mutex_stop);
    
    if (coder->globals->stop)
    {
        pthread_mutex_unlock(&coder->globals->mutex_stop);
        return (1);
    }
    pthread_mutex_unlock(&coder->globals->mutex_stop);
    return (0);
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    if (coder->globals->number_of_compiles_required == 0)
        return (NULL);

    if ((coder->id % 2))
        usleep(200);
    pthread_mutex_lock(&coder->globals->mutex_time);
    coder->last_compile_time = coder->globals->start_time;
    pthread_mutex_unlock(&coder->globals->mutex_time);
    while (1)
    {        
        insert_coder_to_heap(coder->globals->heap, coder);
        
        if (should_stop(coder))
            break;
        
        waiting_to_compile(coder);

        if (should_stop(coder))
            break;

        start_compiling(coder);
        get_last_compile_time(coder);
        release_dongles(coder);        
        increment_total_compiling(coder);

        if (should_stop(coder))
            break;
        
        start_debugging(coder);
        
        if (should_stop(coder))
            break;
        
        start_refactoring(coder);
    }

    return (NULL);
}