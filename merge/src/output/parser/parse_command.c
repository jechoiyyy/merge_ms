/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:43:39 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/12 05:04:25 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_valid_command(t_cmd *cmd)
{
	if (cmd->args)
		return (SUCCESS);
	if (cmd->input_file->filename || cmd->output_file->filename)
		return (SUCCESS);
	if (cmd->hd != -1)
		return (SUCCESS);
	return (FAILURE);	
}

t_cmd	*parse_simple_command(t_token **current, t_prompt *prompt)
{
	t_cmd	*cmd;

	if (!current || !*current)
		return (NULL);
	cmd = create_command(prompt->hd_lst);
	if (!cmd)
		return (NULL);
	while (*current && (*current)->type != T_PIPE)
	{
		if (is_redirect_token((*current)->type))
		{
			if (parse_redirections(current, cmd) == FAILURE)
			{
				free_commands(cmd);
				return (NULL);
			}
		}
		else if ((*current)->type == T_WORD)
		{
			add_argument(cmd, (*current)->value);
			*current = (*current)->next;
		}
		else
			break ;
	}
	if (is_valid_command(cmd) == FAILURE)
	{
		printf("minishell: syntax error near unexpected token\n");
		free_commands(cmd);
		return (NULL);
	}
	return (cmd);
}

int	parse_redirections(t_token **current, t_cmd *cmd)
{
	t_token_type	redir_type;

	if (!current || !*current || !cmd)
		return (FAILURE);
	redir_type = (*current)->type;
	*current = (*current)->next;
	if (!*current || ((*current)->type != T_WORD && \
						(*current)->type != T_CORRECT_FILNAME && \
						(*current)->type != T_WRONG_FILNAME))
	{
		printf("minishell: syntax error near redirection\n");
		return (FAILURE);
	}
	if (redir_type == T_REDIR_IN)
		set_input_file(cmd->input_file, *current);
	else if (redir_type == T_REDIR_OUT)
		set_output_file(cmd, *current, 0);
	else if (redir_type == T_APPEND)
		set_output_file(cmd, *current, 1);
	else if (redir_type == T_HEREDOC)
	{
		// heredoc은 이미 hd_lst에서 처리됨, delimiter만 건너뛰기
	}
	*current = (*current)->next;
	return (SUCCESS);
}
