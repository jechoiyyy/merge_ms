/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:12:54 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 13:33:47 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_numeric(char *str)
{
	if (!str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

static long	ft_atol(const char *s)
{
	long	res;
	int		sign;
	int		i;

	sign = 1;
	res = 0;
	i = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] - '0');
		i++;
	}
	return (res * sign);
}

int	ft_exit(t_cmd *cmd, t_shell *shell)
{
	long	code;

	printf("exit\n");
	if (!cmd->args[1])
		code = shell->last_exit_status;
	else if (!is_numeric(cmd->args[1]))
	{
		print_error("exit", "numeric argument required");
		code = 255;
	}
	else if (cmd->args[2])
	{
		print_error("exit", "too many arguments");
		shell->last_exit_status = 1;
		return (FAILURE);
	}
	else
		code = ft_atol(cmd->args[1]);
	shell->exit_flag = 1;
	shell->last_exit_status = (int)(code % 256);
	cleanup_shell(shell);
	return (SUCCESS);
}
