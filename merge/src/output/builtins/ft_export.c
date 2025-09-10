/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:35:15 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 14:27:33 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	process_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	int		result;

	key = extract_key2(arg);
	if (!key)
		return (FAILURE);
	if (!is_valid_identifier(key))
	{
		print_error("export", "not a valid identifier");
		return (free(key), FAILURE);
	}
	value = extract_value(arg);
	result = set_env_variable(shell, key, value);
	free(key);
	if (value)
		free(value);
	return (result);
}

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	exit_status;

	if (!cmd || !shell)
		return (FAILURE);
	if (!cmd->args[1])
	{
		display_all_exports(shell);
		return (SUCCESS);
	}
	exit_status = SUCCESS;
	i = 1;
	while (cmd->args[i])
	{
		if (process_export_arg(shell, cmd->args[i]) == FAILURE)
			exit_status = FAILURE;
		i++;
	}
	if (exit_status == SUCCESS)
		update_env_array(shell);
	return (exit_status); 
}
 