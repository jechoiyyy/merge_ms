/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:48:52 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/07 19:22:31 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print.h"

static char	*token_type_str(t_token_type type)
{
	if (type == T_WORD) return "WORD";
	if (type == T_PIPE) return "PIPE";
	if (type == T_REDIR_IN) return "REDIR_IN";
	if (type == T_REDIR_OUT) return "REDIR_OUT";
	if (type == T_APPEND) return "APPEND";
	if (type == T_HEREDOC) return "HEREDOC";
	if (type == T_END) return "END";
	if (type == T_ERROR) return "ERROR";
	if (type == T_CORRECT_FILNAME) return "CORRECT_FILENAME";
	if (type == T_WRONG_FILNAME) return "WRONG_FILENAME";
	return "UNKNOWN";
}

void	print_tokens(t_token *tokens, char *step)
{
	printf("***********%s TOKEN LST**********\n", step);
	while (tokens)
	{
		printf("type: %-20s | value: %-10s\n", token_type_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	printf("\n");
}
