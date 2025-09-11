/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ready_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:06:40 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/12 01:28:11 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "setup.h"
#include "utils.h"
#include "sigft.h"
#include <stdio.h>

int	ready_minishell(t_prompt *prompt, t_shell *shell, char **envp)
{
	ft_memset(prompt, 0, sizeof(t_prompt));
	setup_signals();
	prompt->envp_lst = set_envp(envp);
	if (!prompt->envp_lst)
		return (0);
	if (init_shell(shell) != SUCCESS)
		return (printf("Failed to init shell\n"), FAILURE);
	shell->envp_list = prompt->envp_lst;
	shell->env_array = env_list_to_array(shell->envp_list);
	return (1);
}
