/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 16:13:14 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/07 16:55:24 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "global.h"
#include "libft.h"
#include "list.h"

static void	handle_error(t_grammar_status status, t_token *token_lst)
{
	t_token	*err_token;

	if (status == GRAMMAR_EOF_ERROR)
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
	else if (status == GRAMMAR_TOKEN_ERROR)
	{
		err_token = NULL;
		while (token_lst)
		{
			if (token_lst->type == T_ERROR)
			{
				err_token = token_lst;
				break ;
			}
			token_lst = token_lst->next;
		}
		if (err_token)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(err_token->value, 2);
			ft_putstr_fd("'\n", 2);
		}
	}
}

void	syntax_print(t_grammar_status status, t_token **token_lst)
{
	if (!*token_lst)
		return ;
	if (status != GRAMMAR_OK)
	{
		g_exit_status = 2;
		handle_error(status, *token_lst);
		clear_token_list(token_lst);
	}
}
