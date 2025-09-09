/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:26:25 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 17:22:18 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "utils.h"
#include "libft.h"
#include "state.h"

static void	print_quote_error(t_quote_state state)
{
	char	missing_quote;

	missing_quote = 0;
	if (state == STATE_GENERAL)
		return ;
	else
	{
		if (state == STATE_IN_DQUOTE)
			missing_quote = '"';
		else if (state == STATE_IN_SQUOTE)
			missing_quote = '\'';
		ft_putstr_fd("minishell: syntax error: unexpected EOF", 2);
		ft_putstr_fd("while looking for matching '", 2);
		ft_putchar_fd(missing_quote, 2);
		ft_putendl_fd("'", 2);
	}
}

static int	check_quote_error(const char *input)
{
	t_quote_state	state;
	size_t			i;

	state = STATE_GENERAL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			state = state_machine(input[i], state);
		i++;
	}
	if (state != STATE_GENERAL)
	{
		print_quote_error(state);
		return (1);
	}
	return (0);
}

static t_grammar_status	check_pipe(t_token *token)
{
	if (token->next->type == T_END)
	{
		token->type = T_ERROR;
		return (GRAMMAR_EOF_ERROR);
	}
	if (token->next->type == T_PIPE)
	{
		token->next->type = T_ERROR;
		return (GRAMMAR_TOKEN_ERROR);
	}
	return (GRAMMAR_OK);
}

static t_grammar_status	check_redir(t_token *token)
{
	if (token->next->type != T_WORD)
	{
		token->next->type = T_ERROR;
		return (GRAMMAR_TOKEN_ERROR);
	}
	return (GRAMMAR_OK);
}

static t_grammar_status	check_token_error(t_token *token_lst)
{
	t_grammar_status	status;

	while (token_lst && token_lst->type != T_END)
	{
		status = GRAMMAR_OK;
		if (token_lst->type == T_PIPE)
			status = check_pipe(token_lst);
		else if (is_redir_type(token_lst->type))
			status = check_redir(token_lst);
		if (status != GRAMMAR_OK)
			return (status);
		token_lst = token_lst->next;
	}
	return (GRAMMAR_OK);
}

// 문법오류 케이스
// quote 미 닫힘 -> EOF ERROR
// pipe로 끝나기 -> EOF ERROR
// redir연산자 뒤에 word안오기 및 파이프 연속 -> TOKEN ERROR
t_grammar_status	grammar_check(t_prompt *prompt)
{
	t_grammar_status	status;

	if (check_quote_error(prompt->input))
		return (GRAMMAR_EOF_ERROR);
	status = check_token_error(prompt->token_lst);
	return (status);
}
