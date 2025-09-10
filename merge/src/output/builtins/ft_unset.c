/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:35:19 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 13:34:00 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	remove_env_node(t_envp **envp_list, char *key)
{
	t_envp	*current;
	t_envp	*prev;

	if (!envp_list || !*envp_list || !key)
		return ;
	current = *envp_list;
	prev = NULL;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*envp_list = current->next;
			if (current->key)
				free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (!cmd || !shell)
		return (FAILURE);
	if (!cmd->args[1])
		return (SUCCESS);
	i = 1;
	while (cmd->args[i])
	{
		remove_env_node(&shell->envp_list, cmd->args[i]);
		i++;
	}
	return (SUCCESS);
}
