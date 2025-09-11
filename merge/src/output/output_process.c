#include "output.h"
#include "print.h"

int	output_process(t_shell *shell, t_prompt *prompt)
{
	t_cmd	*cmd;

	cmd = parse_tokens(prompt);
	if (!cmd)
		return (printf("Parsing failed\n"), FAILURE);
	print_cmd_list(cmd);
	printf("Starting EXEC\n");
	if (execute_pipeline(cmd, shell) != SUCCESS)
        return (FAILURE);
	free_commands(cmd);
	cleanup_shell(shell);
	return (shell->last_exit_status);
}
