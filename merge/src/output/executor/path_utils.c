/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:57:07 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/11 23:45:19 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*get_path_env(t_shell *shell)
{
	t_envp	*current;

	if (!shell || !shell->envp_list)
		return (NULL);
	current = shell->envp_list;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, "PATH") == 0)
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
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	if (dir[dir_len - 1] != '/')
	{
		full_path[dir_len] = '/';
		full_path[dir_len + 1] = '\0';
	}
	ft_strcat(full_path, command);
	return (full_path);
}
static void	free_array(char **dirs)
{
	int	i;

	i = -1;
	while (dirs[++i])
		free(dirs[i]);
	free(dirs);
}

static char	*search_in_path(char *command, char *path)
{
	char	**dirs;
	char	*full_path;
	int		i;

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = join_path(dirs[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_array(dirs);
			return (full_path);
		}
		if (full_path)
			free(full_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}

char	*find_executable(char *command, t_shell *shell)
{
	char	*path_env;
	char	*executable;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = get_path_env(shell);
	if (!path_env)
		return (NULL);
	executable = search_in_path(command, path_env);
	return (executable);
}