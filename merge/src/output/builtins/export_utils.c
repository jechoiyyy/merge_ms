/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:29:24 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/05 16:10:36 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	update_env_value(t_env *node, char *value)
{
	if (node->value)
		free(node->value);
	if (value)
	{
		node->value = malloc(ft_strlen(value) + 1);
		if (!node->value)
			return (FAILURE);
		ft_strcpy(node->value, value);
	}
	else
		node->value = NULL;
	return (SUCCESS);
}

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = malloc(ft_strlen(key) + 1);
	if (!new_node->key)
		return (free(new_node), NULL);
	ft_strcpy(new_node->key, key);
	if (value)
	{
		new_node->value = malloc(ft_strlen(value) + 1);
		if (!new_node->value)
			return (free(new_node->key), free(new_node), NULL);
		ft_strcpy(new_node->value, value);
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

static void	add_env_to_list(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

int	set_env_variable(t_shell *shell, char *key, char *value)
{
	t_env	*existing;
	t_env	*new_node;

	existing = find_env_node(shell->env_list, key);
	if (existing)
		return (update_env_value(existing, value));
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return (FAILURE);
	add_env_to_list(&shell->env_list, new_node);
	return (SUCCESS);
}

void	update_env_array(t_shell *shell)
{
	if (shell->env_array)
	{
		free(shell->env_array);
		shell->env_array = NULL;
	}
	shell->env_array = env_list_to_array(shell->env_list);
}
