/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 20:05:55 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/09 14:35:01 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	is_redirect_token(t_token_type type)
{
	return (type == 2 || type == 3 \
		|| type == 4 || type == 5);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i = 0;

	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}

	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int si;

	i = 0;
	si = 0;
	while (dest[i] != '\0')
		i++;
	while (src[si] != '\0')
	{
		dest[i] = src[si];
		i++;
		si++;
	}
	dest[i] = '\0';
	return (dest);	
}