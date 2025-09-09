/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:46:49 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 17:00:00 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "input.h"
#include "lexing.h"
#include "expand.h"
#include "split_word.h"
#include "remove.h"
#include "herdoc.h"
#include "print.h"

//입력에 문자가 들어올때까지 계속 이 함수에서 입력받기
int	get_user_input(t_prompt *prompt)
{
	while (1)
	{
		prompt->input = readline("minishell$ ");
		if (!prompt->input)
		{
			printf("exit\n");
			return (0);
		}
		if (*prompt->input == '\0')
		{
			free(prompt->input);
			prompt->input = NULL;
			continue ;
		}
		add_history(prompt->input);
		break ;
	}
	return (1);
}

//렉싱 로직
static t_token	*lex_input(t_prompt *prompt)
{
	t_token	*tokens;

	tokens = input_lexing(prompt->input, 1);
	if (!tokens)
		perror("input_lexing fail");
	else
		print_tokens(tokens, "lexing");
	return (tokens);
}

static int	handle_heredocs(t_prompt *prompt)
{
	if (heredoc_process(prompt) == 0)
		return (0);
	print_herdocs(prompt->hd_lst);
	return (1);
}

static int	expand_and_split(t_prompt *prompt)
{
	if (!expand_token(prompt->token_lst, prompt->envp_lst))
	{
		perror("expand_token fail");
		return (0);
	}
	print_tokens(prompt->token_lst, "expand");
	if (!split_words(&prompt->token_lst))
	{
		perror("split_expanded_words fail");
		return (0);
	}
	print_tokens(prompt->token_lst, "split");
	if (!remove_quote(&prompt->token_lst))
	{
		perror("remove_quote fail");
		return (0);
	}
	print_tokens(prompt->token_lst, "remove");
	return (1);
}

//1. 입력받기
//2. 렉싱하기
//3. 문법검사
//4. herdoc처리
//5. GRAMMAR_OK일때 확장, 분리 및 quote제거
int	input_process(t_prompt *prompt)
{
	t_grammar_status	status;

	if (!get_user_input(prompt))
		return (0);
	prompt->token_lst = lex_input(prompt);
	if (!prompt->token_lst)
		return (0);
	status = grammar_check(prompt);
	print_tokens(prompt->token_lst, "check");
	if (!handle_heredocs(prompt))
		return (0);
	syntax_print(status, &prompt->token_lst);
	if (prompt->token_lst && status == GRAMMAR_OK)
		if (!expand_and_split(prompt))
			return (0);
	return (1);
}
