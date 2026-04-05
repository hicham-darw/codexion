/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darwin <darwin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 01:19:20 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/04/05 03:49:41 by darwin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h> 
# include <string.h>
# include <limits.h>

# define USAGE1 "Usage: ./codexion <number_of_coders> <time_to_burnout>"
# define USAGE2 "<time_to_compile> <time_to_debug> <time_to_refactor>"
# define USAGE3 "<number_of_compiles_required> <dongle_cooldown> <schedular>"
# define ERR_NUMBER_OF_CODERS "invalid argument 0 < <number_of_coders> <= 200\n"
# define ERR_INVALID_ARG "invalid argument 0 < argument <= INT_MAX\n"
# define FIFO "fifo"
# define EDF "edf"
# define TRUE 1
# define FALSE 0

// dongles
typedef struct s_dongle
{
	int				id;
	int				is_taken;
	time_t			cooldown_time;
	pthread_mutex_t	mutex_dongle;
}	t_dongle;

typedef struct s_heap
{
	struct s_coder	**coders;
	int				size;
	int				capacity;
	pthread_mutex_t	mutex_heap;
}	t_heap;

typedef struct s_manager
{
	pthread_t			thread;
	t_dongle			**dongles;
	struct s_global		*globals;
	t_heap				*heap;
	pthread_mutex_t		mutex_heap;
}	t_manager;

typedef struct s_global
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	char				*scheduler;
	time_t				start_time;
	int					stop;
	struct s_coder		*coders;
	struct s_dongle		*dongles;
	struct s_monitor	*monitor;
	struct s_manager	*manager;
	struct s_heap		*heap;
	pthread_mutex_t		mutex_stop;
	pthread_mutex_t		mutex_time;
	pthread_mutex_t		mutex_print;
}	t_global;


typedef struct s_monitor
{
	pthread_t			thread;
	struct s_coder		**coders;
	struct s_global		*globals;
}	t_monitor;

typedef struct s_coder
{
	pthread_t			thread;
	int					id;
	int					total_compiling;
	int					stop;
	int					can_compile;
	int					is_compiling;
	time_t				deadline;
	time_t				arrival;	
	time_t				last_compile;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_global			*globals;
	pthread_mutex_t		mutex_coder;
	pthread_cond_t		cond_coder;
}	t_coder;

// helper functions
size_t		ft_strlen(const char *str);
long long	ft_atoll(const char *str);
char		*ft_strdup(const char *str);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strtrim(const char *str);
char		*ft_strtolower(char *str);
void		*ft_memalloc(size_t size);

// parser functions
t_global	*parse_arguments(char **argv);

int			initializer(t_global **global_var);
t_dongle	*initial_dongles(int number_of_coders);
t_coder		*initial_coders(t_global **global_var);
t_monitor	*initial_monitor(t_global *global_var);
t_heap		*initial_heap(t_global **global_var);

// threads_functions
int			create_threads(t_global *global_var);
int			join_threads(t_global *globals_var);

// routine functions
void		*coder_routine(void *args);
void		*monitor_routine(void *args);
void		*manager_routine(void *args);

// times functions
time_t		get_time_by_milisecond(void);

// free memory
void		free_global_var(t_global *global_var);
void		free_coders(t_coder *coder, int number_of_coders);
void		free_dongles(t_dongle *dongles, int number_of_coders);
void		free_manager(t_manager *manager);
void		free_monitor(t_monitor *monitor);
void		destroy_mutexes_of_coders_at(t_coder *coders, int i);
void		destroy_conds_of_coders_at(t_coder *coders, int i);
void		destroy_mutexes_of_dongles_at(t_dongle *dongle, int i);

// manager functions
t_manager	*initial_manager(t_global *global);

// heap functions
void		insert_coder_to_heap(t_heap *heap, t_coder *coder);
void		heapify_down_by_edf(t_heap *heap, int index);
void		heapify_down_by_fifo(t_heap *heap, int index);
void		heapify_up_by_edf(t_heap *heap, int index);
void		heapify_up_by_fifo(t_heap *heap, int index);
t_coder		*pop_heap_at(t_heap *heap, int index);
int			is_empty_heap(t_heap *heap);
void		free_heap(t_heap *heap);
int			is_in_heap(t_heap *heap, t_coder *coder);

// coder functions
void		swap_coders(t_coder **a, t_coder **b);
int			burnout_coder(t_coder *coder);
void		increment_total_compiling(t_coder *coder);
void		get_last_compile_time(t_coder *coder);
int			waiting_to_compile(t_coder *coder);
void		start_compiling(t_coder *coder);
int			should_finish(t_coder *coder);
int			join_coders_at(t_coder *coders, int number_of_coders);

// dongles functions
void		release_dongles(t_coder *coder);
int			take_dongles(t_coder *coder);

// print_actions_precise
void		print_action(t_coder *coder, char *msg);
void		precise_sleep(long time_ms);

// stop
void	change_stop_var(t_monitor *monitor);

#endif
