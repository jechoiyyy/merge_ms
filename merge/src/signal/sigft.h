#ifndef SIGFT_H
# define SIGFT_H

# include "types.h"
# include <signal.h>

# define SIG_INTERACTIVE 1
# define SIG_NON_INTERACTIVE 0
# define SIG_CHILD 2

void	setup_signals(void);
void	signal_handler_interactive(int sig);
void	signal_handler_noninteractive(int sig);
void	setup_signals_interactive(void);
void	setup_signals_noninteractive(void);
void	setup_signals_child(void);
void	handle_signal_in_loop(t_shell *shell);

#endif