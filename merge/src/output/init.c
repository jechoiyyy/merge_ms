/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:05:00 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/12 01:27:50 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "output.h"
#include "utils.h"

static char	*extract_key(char *env_str, char *equal_sign)
{
	char	*key;
	size_t	key_len;

	key_len = equal_sign - env_str;
	key = malloc(key_len + 1);
	if (!key)
		return (NULL);
	ft_strncpy(key, env_str, key_len);
	key[key_len] = '\0';
	return (key);
}

t_envp	*create_env_node(char *env_str)
{
	t_envp	*node;
	char	*equal_sign;

	if (!env_str)
		return (NULL);
	node = malloc(sizeof(t_envp));
	if (!node)
		return (NULL);
	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
	{
		node->key = ft_strdup(env_str);
		node->value = ft_strdup("");
	}
	else
	{
		node->key = extract_key(env_str, equal_sign);
		node->value = ft_strdup(equal_sign + 1);
	}
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	add_env_node(t_envp **head, t_envp *new_node)
{
	t_envp	*current;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

static char	*create_env_string(t_envp *current)
{
	size_t	key_len;
	size_t	val_len;
	char	*env_str;

	key_len = ft_strlen(current->key);
	val_len = ft_strlen(current->value);
	env_str = malloc(key_len + val_len + 2);
	if (!env_str)
		return (NULL);
	ft_strcpy(env_str, current->key);
	ft_strcat(env_str, "=");
	ft_strcat(env_str, current->value);
	return (env_str);
}

char	**env_list_to_array(t_envp *envp_list)
{
	t_envp	*current;
	char	**array;
	int		count;
	int		i;

	count = 0;
	current = envp_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	current = envp_list;
	while (current)
	{
		array[i] = create_env_string(current);
		if (!array[i])
		{
			while (i > 0)
				free(array[--i]);
			free(array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

int init_shell(t_shell *shell)
{
    if (!shell)
        return (FAILURE);
    shell->envp_list = NULL;
    shell->env_array = NULL;
    shell->last_exit_status = 0;
    shell->exit_flag = 0;
    shell->signal_mode = SIG_INTERACTIVE;
    return (SUCCESS);
}
