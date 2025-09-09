#include "output.h"
#include "print.h"

int	output_process(t_shell *shell, t_prompt *prompt)
{
	t_cmd	*cmd;

	if (init_shell(shell, prompt->envp_lst) != SUCCESS)
		return (printf("Failed to init shell\n"), FAILURE);
	cmd = parse_tokens(prompt);
	if (!cmd)
		return (printf("Parsing failed\n"), FAILURE);
	print_cmd_list(cmd);
	// if (execute_pipeline(cmd, shell) == FAILURE)
    //     return (FAILURE);
	free_commands(cmd);
	cleanup_shell(shell);
	return (SUCCESS);
}
