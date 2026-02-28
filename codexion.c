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
    global_var->dongles = initial_dongles(global_var->number_of_coders);
    if (!global_var->dongles)
    {
        free_global_var(global_var);
        return (42);
    }
    global_var->coders = initial_coders(&global_var);
    if (!global_var->coders)
    {
        free_global_var(global_var);
        return 42;
    }

    i = 0;
    while (i < global_var->number_of_coders)
    {
        printf("coder id : %d\n", global_var->coders[i].id);
        printf("dongle left  id: %d\n", global_var->coders[i].left_dongle->id);
        printf("dongle right id: %d\n", global_var->coders[i].right_dongle->id);
        printf("-------------------------------->\n");
        i ++;
    }
    i = 0;
    while (i < global_var->number_of_coders)
    {
        if(pthread_create(&(global_var->coders[i].thread), NULL, &start_routine, &(global_var->coders[i])) != 0)
        {
            free_global_var(global_var);
            return (42);
        }
        i += 1;
    }
    i = 0;
    while (i < global_var->number_of_coders)
    {
        pthread_join(global_var->coders[i].thread, NULL);
        i += 1;
    }
    global_var->monitor = initial_monitor();
    if (!global_var->monitor)
    {
        free_global_var(global_var);
        return (42);
    }
    if (pthread_create(&(global_var->monitor->thread), NULL, monitor_routine, NULL) != 0)
    {
        free_global_var(global_var);
        return 42;
    }
    pthread_join(global_var->monitor->thread, NULL);
    free_global_var(global_var);
    return 0;
}