/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:31:12 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 13:16:10 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd	*parse_tokens(t_prompt *prompt)
{
	t_token		*tokens;
	t_token		*current;
	t_cmd		*commands;

	tokens = prompt->token_lst;
	if (!tokens)
		return (NULL);
	current = tokens;
	commands = parse_pipeline(&current, prompt);
	return (commands);
}

t_cmd	*parse_pipeline(t_token **current, t_prompt *prompt)
{
	t_cmd	*first_cmd;
	t_cmd	*last_cmd;
	t_cmd	*new_cmd;

	if (!current || !*current)
		return (NULL);
	first_cmd = parse_simple_command(current, prompt);
	if (!first_cmd)
		return (NULL);
	last_cmd = first_cmd;
	printf("DEBUG: Starting Parse_pipeline\n");
	printf("DEBUG: First Command parsed\n");
	printf("%s\n", (*current)->value);
	while (*current && (*current)->type == T_PIPE)
	{
		printf("DEBUG: Found pipe, parsing next command\n");
		*current = (*current)->next;
		if (!*current)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			free_commands(first_cmd);
			return (NULL);
		}
		new_cmd = parse_simple_command(current, prompt);
		if (!new_cmd)
		{
			free_commands(first_cmd);
			return (NULL);
		}
		printf("DEBUG: Linked new cmd to pipeline\n");
		last_cmd->next = new_cmd;
		last_cmd = new_cmd;
	}
	printf("DEBUG: Pipeline parsing complete, returning first_cmd\n");
		return (first_cmd);
}