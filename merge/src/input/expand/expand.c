/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:11:42 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 17:20:57 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "input.h"
#include <stdio.h>
#include "libft.h"
#include "state.h"
#include "global.h"

//확장로직
char	*expand_process(char *value, t_envp *envp_lst)
{
	t_exp	exp_data;

	if (!set_exp_data(&exp_data))
		return (NULL);
	while (value[exp_data.idx])
	{
		if (is_valid_expand(&exp_data, value))
		{
			if (!exp_env_process(&exp_data, value, envp_lst))
				return (NULL);
			continue ;
		}
		if (is_quote(value[exp_data.idx]))
			exp_data.state = state_machine(value[exp_data.idx], exp_data.state);
		if (!exp_normal_process(&exp_data, value))
			return (NULL);
		exp_data.idx++;
	}
	return (exp_data.result);
}

static int	expand_word_token(t_token *curr, t_envp *envp_lst)
{
	char	*temp;

	if (!curr || !curr->value || !envp_lst || curr->type == T_END)
	{
		perror("unexpected error : expand_word_token");
		return (0);
	}
	temp = expand_process(curr->value, envp_lst);
	if (!temp)
	{
		perror("malloc fail : expand_process");
		return (0);
	}
	free (curr->value);
	curr->value = temp;
	return (1);
}

int	has_whitespace(const char *s)
{
	t_quote_state	state;
	size_t			i;

	if (!s)
		return (0);
	state = STATE_GENERAL;
	i = 0;
	while (s[i])
	{
		if (state == STATE_GENERAL && is_space((unsigned char)s[i]))
			return (1);
		if (s[i] == '\'' || s[i] == '"')
			state = state_machine(s[i], state);
		i++;
	}
	return (0);
}

typedef struct s_expand_ctx
{
	int	do_check_filename;
	int	ambiguous_printed;
}	t_expand_ctx;

static void	print_ambiguous_error(char *value)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(value, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
}

//quote 일반 상태일때 whitespcae 존재시 파일문법 오류
static int	handle_word(t_token *curr, t_envp *envp_lst, t_expand_ctx *ctx)
{
	char	*orig_value;

	orig_value = ft_strdup(curr->value);
	if (!orig_value)
		return (0);
	if (!expand_word_token(curr, envp_lst))
		return (free(orig_value), 0);
	if (ctx->do_check_filename)
	{
		if (curr->value[0] == '\0' || has_whitespace(curr->value))
		{
			curr->type = T_WRONG_FILNAME;
			if (!ctx->ambiguous_printed)
			{
				print_ambiguous_error(orig_value);
				ctx->ambiguous_printed = 1;
			}
		}
		else
			curr->type = T_CORRECT_FILNAME;
		ctx->do_check_filename = 0;
	}
	free(orig_value);
	return (1);
}

//변수 확장 및 filename 타입 세팅
//파일이름 문법 오류시 에러메세지 파이프 별로 하나씩 출력
//잘못된 파일인 경우 token type으로 명시
int	expand_token(t_token *token_lst, t_envp *envp_lst)
{
	t_token			*curr;
	t_expand_ctx	ctx;

	curr = token_lst;
	ctx.do_check_filename = 0;
	ctx.ambiguous_printed = 0;
	while (curr && curr->type != T_END)
	{
		if (curr->type == T_HEREDOC)
			curr = curr->next;
		else if (curr->type == T_REDIR_IN || curr->type == T_REDIR_OUT
			|| curr->type == T_APPEND)
			ctx.do_check_filename = 1;
		else if (curr->type == T_WORD)
		{
			if (!handle_word(curr, envp_lst, &ctx))
				return (0);
			ctx.do_check_filename = 0;
		}
		else if (curr->type == T_PIPE)
			ctx.ambiguous_printed = 0;
		curr = curr->next;
	}
	return (1);
}
