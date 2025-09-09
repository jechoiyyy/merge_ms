/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:57:07 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/09 12:32:23 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*get_path_env(t_shell *shell)
{
	t_envp	*current;

	if (!shell || !shell->env_list)
		return (NULL);
	current = shell->env_list;
	while (current)
	{
		if (current->key && strcmp(current->key, "PATH") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*join_path(char *dir, char *command)
{
	char	*full_path;
	int		dir_len;
	int		cmd_len;

	if (!dir || !command)
		return (NULL);
	dir_len = strlen(dir);
	cmd_len = strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	strcpy(full_path, dir);
	if (dir[dir_len - 1] != '/')
	{
		full_path[dir_len] = '/';
		full_path[dir_len + 1] = '\0';
	}
	strcat(full_path, command);
	return (full_path);
}

static char	*search_in_path(char *command, char *path)
{
	char	*path_copy;
	char	*dir;
	char	*full_path;

	path_copy = malloc(strlen(path) + 1);
	if (!path_copy)
		return (NULL);
	strcpy(path_copy, path);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = join_path(dir, command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		if (full_path)
			free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

char	*find_executable(char *command, t_shell *shell)
{
	char	*path_env;
	char	*executable;

	if (!command)
		return (NULL);
	if (strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}
	path_env = get_path_env(shell);
	if (!path_env)
		return (NULL);
	executable = search_in_path(command, path_env);
	return (executable);
}