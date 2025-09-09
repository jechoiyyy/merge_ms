#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "minishell.h"

# define READ_END 0
# define WRITE_END 1

int		execute_pipeline(t_cmd *commands, t_shell *shell);
int		execute_command(t_cmd *cmd, t_shell *shell);
int		execute_builtin(t_cmd *cmd, t_shell *shell);
int		execute_external(t_cmd *cmd, t_shell *shell);
pid_t	fork_process(void);
int		wait_for_children(pid_t *pids, int count);
int		wait_for_children(pid_t *pids, int count);
void	setup_child_process(t_cmd *cmd, int *pipe_fds, int cmd_index, int cmd_count);
void	setup_parent_process(int *pipe_fds, int cmd_index, int cmd_count);
void	close_all_pipes(int *pipe_fds, int pipe_count);
int		count_commands(t_cmd *cmd);
int		setup_redirections(t_cmd *cmd);
int		open_input_file(char *filename);
int		open_output_file(char *filename, int append_mode);
int		setup_heredoc(char *delimiter);


// Builtins
int		is_builtin_command(char *cmd);
int		ft_cd(t_cmd *cmd, t_shell *shell);
int		ft_echo(t_cmd *cmd);
int		ft_env(t_shell *shell);
int		ft_exit(t_cmd *cmd, t_shell *shell);
int		ft_export(t_cmd *cmd, t_shell *shell);
void	display_all_exports(t_shell *shell);
int	set_env_variable(t_shell *shell, char *key, char *value);
void	update_env_array(t_shell *shell);
int		ft_pwd(void);
int		ft_unset(t_cmd *cmd, t_shell *shell);

int		is_valid_identifier(char *name);
char	*extract_key(char *arg);
char	*extract_value(char *arg);
t_env	*find_env_node(t_env *env_list, char *key);
char	*get_path_env(t_shell *shell);
char	*find_executable(char *command, t_shell *shell);

#endif
