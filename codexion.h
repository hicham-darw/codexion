#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h> 
# include <string.h>
# include <limits.h>


// dongles
typedef struct s_dongle
{
    int     id;
    int     is_taken;
    
    struct timeval last_compile;
        
    pthread_mutex_t mutex_dongle;
}t_dongle;

typedef struct s_req
{
    struct s_coder **coder;

    long    *arrival_time;
    long    *deadline_time;
}   t_req;

typedef struct s_heap
{
    struct s_coder  **coders;
    
    int size;
    int capacity;

    pthread_mutex_t mutex_heap;
    pthread_cond_t cond_heap;
}   t_heap;

typedef struct s_manager
{
    pthread_t thread;
    
    t_dongle **dongles;

    struct s_global    *globals;
    t_heap  *heap;
}   t_manager;

typedef struct s_global
{
    int     number_of_coders;
    int     time_to_burnout;
    int     time_to_compile;
    int     time_to_debug;
    int     time_to_refactor;
    int     number_of_compiles_required;
    int     dongle_cooldown;
    char    *schedular;

    time_t  start_time;
    
    int     is_finished;
    struct s_coder *coders;
    struct s_dongle *dongles;
    struct s_monitor *monitor;
    struct s_manager *manager;
    struct s_heap   *heap;
    pthread_mutex_t mutex_time;
    pthread_mutex_t mutex_print;

}t_global;

typedef struct s_monitor
{
    pthread_t   thread;
    int is_stop;
}t_monitor;

typedef struct s_coder
{
    pthread_t   thread;
    int     id;

    int both_available;
    int is_compiling;
    int is_debugging;
    int is_refactoring;
    int number_of_compilling;

    time_t last_compile_time;
    time_t  start_time;

    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    t_global    *globals;

    pthread_mutex_t mutex_coder;
    pthread_cond_t cond_coder;
}t_coder;

// helper functions
size_t ft_strlen(const char *str);
long long     ft_atoll(const char *str);
char    *ft_strdup(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
char    *ft_strtrim(const char *str);
char    *ft_strtolower(char *str);
int     valid_schedular(char *schedular);

// parser functions
t_global    *parse_arguments(char **argv);
int     add_parsed_number(t_global **storage, int number, int index);

// initialize functions
int     initializer(t_global **global_var);
t_dongle    *initial_dongles(int number_of_coders);
t_coder     *initial_coders(t_global **global_var);
t_monitor   *initial_monitor(void);

// creater functions
int     create_threads(t_global *global_var);
int     join_threads(t_global *globals_var);
// routine function
void    *coder_routine(void *args);
void    *monitor_routine(void *args);
void    *manager_routine(void *args);

// times function
time_t  get_time_by_milisecond(void);
// free memory
void free_data_input(t_global *args);
void free_global_var(t_global *global_var);
void    free_coders(t_coder *coder, int number_of_coders);
void    free_dongles(t_dongle *dongles, int number_of_coders);
void    free_manager(t_manager * manager);

//manager functions
t_manager   *initial_manager(t_global *global);

// heap functions
void    insert_coder_to_heap(t_heap *heap, t_coder *coder);
void heapify_down(t_heap *heap, int index);
void heapify_up(t_heap *heap, int index);
t_coder *pop_heap_at(t_heap *heap, int index);
void    push_to_heap(t_heap *heap, t_coder *coder);


// coder functions
void swap_coders(t_coder **a, t_coder **b);

//dongles functions
int    is_dongles_available(t_coder *coder);

// print_log
void	print_log(t_coder *coder, char *msg);
void precise_sleep(long time_ms);


#endif