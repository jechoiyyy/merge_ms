/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:58:34 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/12 00:06:30 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	if (setup_redirections(cmd, 0, 1) == FAILURE)
		return (FAILURE);
	if (is_builtin_command(cmd->args[0]))
		return (execute_builtin(cmd, shell));
	return (execute_external(cmd, shell));
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	char	*command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	command = cmd->args[0];
	if (ft_strcmp(command, "echo") == 0)
		return (ft_echo(cmd));
	if (ft_strcmp(command, "cd") == 0)
		return (ft_cd(cmd, shell));
	if (ft_strcmp(command, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(command, "export") == 0)
		return (ft_export(cmd, shell));
	if (ft_strcmp(command, "unset") == 0)
		return (ft_unset(cmd, shell));
	if (ft_strcmp(command, "env") == 0)
		return (ft_env(shell));
	if (ft_strcmp(command, "exit") == 0)
		return (ft_exit(cmd, shell));
	return (FAILURE);
}

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*executable_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	executable_path = find_executable(cmd->args[0], shell);
	if (!executable_path)
	{
		print_error(cmd->args[0], "command not found");
		return (127);
	}
	if (execve(executable_path, cmd->args, shell->env_array) == -1)
	{
		print_error(cmd->args[0], strerror(errno));
		free(executable_path);
		return (126);
	}
	return (SUCCESS);
}
