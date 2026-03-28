#include "codexion.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int	real_number(const char *str)
{
	if (!str || !*str)
		return (0);
	while (*str && *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			return (0);
		str++;
	}
	if (*str < '0' || *str > '9')
		return (0);
	while (*str && *str >= '0' && *str <= '9')
		str++;
	while (*str && *str == ' ')
		str++;
	if (!*str)
		return (1);
	return (0);
}

void	*ft_memalloc(size_t size)
{
	void	*ptr;

	if (size <= 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, size);
	return (ptr);
}

long long	ft_atoll(const char *str)
{
	long long	res;

	if (!real_number(str))
		return (-1);
	res = 0;
	while (*str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			return (-1);
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		if (res > INT_MAX)
			return (-1);
		res = (res * 10) + (*str - '0');
		str++;
	}
	return (res);
}

