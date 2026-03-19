# include "codexion.h"

int main(int ac, char **av)
{
    t_global *global_var;
    t_dongle *dongle;

    int     i;

    if (ac != 9)
    {
        perror("Usage: ./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <schedular>");
        return (42);
    }
    global_var = parse_arguments(av + 1);
    if (!global_var)
        return (42);
    if (
        !initializer(&global_var)
        || !create_threads(global_var)
        || !join_threads(global_var)
    )
        return (42);

    pthread_mutex_destroy(&global_var->mutex_print);
    pthread_mutex_destroy(&global_var->mutex_print);
    free_global_var(global_var);
    return 0;
}