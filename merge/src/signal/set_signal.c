/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:59:43 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 16:30:53 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

static void	handler_main(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_main_signal(void)
{
	signal(SIGINT, handler_main);
	signal(SIGQUIT, SIG_IGN);
}
