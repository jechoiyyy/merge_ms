/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charjoin_and_free.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 19:40:29 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/05 17:35:55 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*charjoin_and_free(char *s, char c)
{
	char	*result;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	result = malloc(len + 2);
	if (!result)
		return (free(s), NULL);
	ft_memcpy(result, s, len);
	result[len] = c;
	result[len + 1] = '\0';
	return (free(s), result);
}
