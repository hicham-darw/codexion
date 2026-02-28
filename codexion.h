#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h> 
# include <stdbool.h>
# define INT_MAX 2147483647

// dongles
typedef struct s_dongle
{
    int     id;
    int     is_taken;
    pthread_mutex_t lock;
}t_dongle;


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

    struct timeval  *start_time;
    struct s_coder *coders;
    struct s_dongle *dongles;
    struct s_monitor *monitor;

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
    int is_compiling;
    int is_debugging;
    int is_refactoring;

    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    t_global    *globals;

}t_coder;

// helper functions
size_t ft_strlen(const char *str);
long long     ft_atoll(const char *str);
char    *ft_strdup(const char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char    *ft_strtrim(const char *str);

// parser functions
t_global    *parse_arguments(char **argv);
int     add_parsed_number(t_global **storage, int number, int index);

// initialize functions
t_dongle    *initial_dongles(int number_of_coders);
t_coder     *initial_coders(t_global **global_var);
t_monitor   *initial_monitor(void);
// routine function
void    *start_routine(void *args);
void    *monitor_routine(void *args);

// free memory
void free_data_input(t_global *args);
void free_global_var(t_global *global_var);
void    free_coders(t_coder *coder);
void    free_dongles(t_dongle *dongles);

#endif