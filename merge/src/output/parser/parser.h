#ifndef PARSER_H
# define PARSER_H

# include "types.h"
# include "libft.h"
# include "utils.h"
# include "stdio.h"

t_cmd	*create_command(t_hd *hd_lst);
void 	add_argument(t_cmd *cmd, char *arg);
int		is_redirect_token(t_token_type type);
t_cmd	*parse_tokens(t_prompt *prompt);
t_cmd	*parse_pipeline(t_token **current, t_prompt *prompt);
t_cmd	*parse_simple_command(t_token **current, t_prompt *prompt);
int		parse_redirections(t_token **current, t_cmd *cmd);
void	set_input_file(t_filename *file, t_token *current);
void	set_output_file(t_cmd *cmd, t_token *current, int append);
// void	set_heredoc_delimiter(t_cmd *cmd, char *delimiter);
void	free_commands(t_cmd *commands);
t_filename	*create_filename(void);

#endif