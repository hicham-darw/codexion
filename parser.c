#include "codexion.h"


int    add_parsed_number(t_global **global, int number, int index)
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

t_global    *parse_arguments(char **av)
{
    long long     parsed_number;
    t_global     *valid_input;
    int     i;

    if (!av || !*av)
        return (NULL);
    valid_input = malloc(sizeof(t_global));
    if (!valid_input)
        return (NULL);
    valid_input->schedular = NULL;
    valid_input->coders = NULL;
    valid_input->dongles = NULL;
    i = 0;
    while (av[i] && i < 7)
    {
        parsed_number = ft_atoll(av[i]);
        if (parsed_number < 0 || parsed_number == 0 && i == 0 || parsed_number > INT_MAX)
        {
            free_global_var(valid_input);
            perror("args cannot be negative number or zero");
            return (NULL);
        }
        if (add_parsed_number(&valid_input, parsed_number, i) <= 0)
        {
            free_global_var(valid_input);
            return (NULL);
        }
        printf("parsed number %d: %lld\n", i, parsed_number);
        i += 1;
    }
    char *schedular = ft_strtrim(av[i]);
    if (
        !schedular
        || (ft_strncmp(schedular, "fifo", ft_strlen(schedular))
        && ft_strncmp(schedular, "edf", ft_strlen(schedular)))
    )
    {
        if (schedular)
            free(schedular);
        free_global_var(valid_input);
        fprintf(stderr, "Error: schedular must be <fifo> or <edf>");
        return (NULL);
    }
    valid_input->schedular = schedular;
    return (valid_input);
}