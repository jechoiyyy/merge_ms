/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ready_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:06:40 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/10 16:10:54 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "setup.h"
#include "sigft.h"

int	ready_minishell(t_prompt *prompt, char **envp)
{
	ft_memset(prompt, 0, sizeof(t_prompt));
	setup_signals();
	prompt->envp_lst = set_envp(envp);
	if (!prompt->envp_lst)
		return (0);
	return (1);
}
