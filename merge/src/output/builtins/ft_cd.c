/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:16:27 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 14:28:54 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static char	*get_home_path(t_shell *shell)
{
	t_envp	*current;

	if (!shell || !shell->envp_list)
		return (NULL);
	current = shell->envp_list;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, "HOME") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd || !shell)
		return (FAILURE);
	if (cmd->args[1] == NULL || ft_strlen(cmd->args[1]) == 0)
	{
		path = get_home_path(shell);
		if (!path)
			return (print_error("cd", "HOME not set"), FAILURE);
	}
	else if (cmd->args[2])
		return (print_error("cd", "too many args"), FAILURE);
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
	{
		print_error("cd", strerror(errno));
		return (FAILURE);
	}
	return (SUCCESS);
}
