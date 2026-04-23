/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-hamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 01:11:19 by hel-hamo          #+#    #+#             */
/*   Updated: 2026/03/30 01:12:55 by hel-hamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

char	*ft_strtolower(char *str)
{
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (ft_isupper(str[i]))
			str[i] += 32;
		i += 1;
	}
	return (str);
}

char	*ft_strdup(const char *str)
{
	char	*ptr;
	int		i;

	if (!str)
		return (NULL);
	ptr = ft_memalloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strtrim(const char *str)
{
	char	*ptr;
	int		start;
	int		last;
	int		i;

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
		ptr[i++] = str[start++];
	ptr[i] = 0;
	return (ptr);
}
