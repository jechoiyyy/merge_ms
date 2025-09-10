/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:32:35 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 14:27:09 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!((name[0] >= 'a' && name[0] <= 'z') || \
			(name[0] >= 'A' && name[0] <= 'Z') || \
			name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z') || \
			(name[i] >= 'A' && name[i] <= 'Z') || \
			(name[i] >= '0' && name[i] <= '9') || \
			name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

char	*extract_key2(char *arg)
{
	char	*equal_pos;
	char	*key;
	int		len;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		len = equal_pos - arg;
	else
		len = ft_strlen(arg);
	key = malloc(len + 1);
	if (!key)
		return (NULL);
	ft_strncpy(key, arg, len);
	key[len] = '\0';
	return (key);
}

char	*extract_value(char *arg)
{
	char	*equal_pos;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (NULL);
	value = malloc(ft_strlen(equal_pos));
	if (!value)
		return (NULL);
	ft_strcpy(value, equal_pos + 1);
	return (value);
}

t_envp	*find_env_node(t_envp *envp_list, char *key)
{
	t_envp	*current;

	current = envp_list;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
