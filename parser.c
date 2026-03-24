#include "codexion.h"


static int    add_parsed_number(t_global **global, int number, int index)
{
    if (!global)
        return (0);
    if (index == 0) // number of coders
        (*global)->number_of_coders = number;
    else if (index == 1) // time burnout 
        (*global)->time_to_burnout = number;
    else if (index == 2) // time to compile
        (*global)->time_to_compile = number;
    else if (index == 3) // time debug
        (*global)->time_to_debug = number;
    else if (index == 4) // time refactor
        (*global)->time_to_refactor = number;
    else if (index == 5) // number_of_compiles_required
        (*global)->number_of_compiles_required = number;
    else if (index == 6) // dongle cooldown
        (*global)->dongle_cooldown = number;
    return 1;
}

static t_global    *parse_numbers(t_global *valid_input, char **av)
{
    long long   parsed_number;
    int     i;

    i = 0;
    while (av[i] && i < 7)
    {
        parsed_number = ft_atoll(av[i]);
        if (parsed_number < 0 || (parsed_number == 0 && i == 0) || parsed_number > INT_MAX)
        {
            free_global_var(valid_input);
            if (i == 0)
                fprintf(stderr, "invalid argument 0 < <number_of_coders> <= 200\n");
            else
                fprintf(stderr, "invalid argument 0 < argument <= INT_MAX\n");
            return (NULL);
        }
        if (add_parsed_number(&valid_input, (int)parsed_number, i) <= 0)
        {
            free_global_var(valid_input);
            return (NULL);
        }
        i += 1;
    }
    return (valid_input);
}

static char    *parse_schedular(char *arg)
{
    char    *schedular;
    int     cmp_1;
    int     cmp_2;

    schedular = ft_strtrim(arg);
    if (!schedular)
        return (NULL);

    cmp_1 = ft_strcmp(ft_strtolower(schedular), "fifo");
    cmp_2 = ft_strcmp(ft_strtolower(schedular), "edf");

    if (cmp_1 && cmp_2)
    {
        free(schedular);
        schedular = NULL;
        return (NULL);
    }
    return (schedular);
}


t_global    *parse_arguments(char **av)
{
    t_global     *valid_input;

    if (!av || !*av)
        return (NULL);
    valid_input = malloc(sizeof(t_global));
    if (!valid_input)
        return (NULL);
    memset(valid_input, 0, sizeof(t_global));
    valid_input = parse_numbers(valid_input, av);
    if (!valid_input)
        return (NULL);
    valid_input->schedular = parse_schedular(av[7]);
    if (!valid_input->schedular)
    {
        free_global_var(valid_input);
        return (NULL);
    }

    return (valid_input);
}