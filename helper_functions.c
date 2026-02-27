#include "codexion.h"

size_t ft_strlen(const char *str)
{
    int     i;

    i = 0;
    while (str[i])
        i++;
    return i;
}

int     ft_isdigit(int c)
{
    return (c >= 48 && c <= 57);
}

long long     ft_atoll(const char *str)
{
    long long     res;


    res = 0;
    while(*str == ' ')
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            return -1;
        str++;
    }
    while (*str)
    {
        if (!ft_isdigit(*str))
            return -1;
        while (*str >= '0' && *str <= '9')
        {
            res = (res * 10) + (*str - '0');
            str++;
        }
    }
    return res;
}

char    *ft_strdup(const char *str)
{
    char    *ptr;
    int     i;

    if (!str)
        return NULL;
    ptr = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if (!ptr)
        return NULL;
    i = 0;
    while (str[i])
    {
        ptr[i] = str[i];
        i++;
    }
    ptr[i] = 0;
    return (ptr);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] == s2[i]) && s1[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


char    *ft_strtrim(const char *str)
{
    char    *ptr;
    int     start;
    int     last;
    int     i;

    if (!str)
        return (NULL);
    start = 0;
    last = ft_strlen(str) - 1;
    while (str[start] == ' ')
        start ++;
    while (last >= 0 && str[last] == ' ')
        last--;
    if (last < start)
        return (NULL);
    ptr = malloc(sizeof(char) * (last - start) + 2);
    if (!ptr)
        return (NULL);
    i = 0;
    while (str[start] && start <= last)
    {
        ptr[i] = str[start];
        i += 1;
        start += 1;
    }
    ptr[i] = 0;
    return ptr;
}


void free_data_input(t_global *data)
{
    if (!data)
        return;
    if (data->schedular)
        free(data->schedular);
    if (data->coders)
        free(data->coders);
    free(data);
}