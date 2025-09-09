/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 10:49:24 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/09 20:00:19 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "utils.h"

void	free_env_list(t_envp *head)
{
	t_envp	*current;
	t_envp	*next;
	
	current = head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	shell->envp_list = NULL;
	if (shell->env_array)
	{
		free_env_array(shell->env_array);
		shell->env_array = NULL;
	}
}

void	print_error(char *cmd, char *msg)
{
	if (cmd && msg)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ": : ", 2);
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
	}
}