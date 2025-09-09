/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ready_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:06:40 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/05 17:35:06 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "setup.h"
#include "sigft.h"

int	ready_minishell(t_prompt *prompt, char **envp)
{
	ft_memset(prompt, 0, sizeof(t_prompt));
	set_main_signal();
	prompt->envp_lst = set_envp(envp);
	if (!prompt->envp_lst)
		return (0);
	return (1);
}
