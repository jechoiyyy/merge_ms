/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:21:27 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 16:13:54 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sigft.h"
#include <termios.h>
#include <readline/readline.h>
#include <stdio.h>

volatile sig_atomic_t g_signal_received = 0;

// void	disable_echoctl(void)
// {
// 	struct termios	term;

// 	if (tcgetattr(STDIN_FILENO, &term) == -1)
// 		return ;
// 	term.c_lflag &= ~ECHOCTL;
// 	tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

void	setup_signals(void)
{
	//disable_echoctl();
	setup_signals_interactive(); 
}

void	signal_handler_interactive(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_handler_noninteractive(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
		printf("\n");
	else if (sig == SIGQUIT)
		printf("Quit: %d\n", sig);
}