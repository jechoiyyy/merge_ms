/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:06:19 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 11:33:13 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\f' || c == '\n' || c == '\r'
		|| c == '\t' || c == '\v');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_redir_type(t_token_type type)
{
	return (type == T_APPEND || type == T_HEREDOC || \
			type == T_REDIR_IN || type == T_REDIR_OUT);
}

int	is_valid_expand(t_exp *exp_data, char *value)
{
	if (value[exp_data->idx] != '$')
		return (0);
	if (exp_data->state == STATE_IN_SQUOTE)
		return (0);
	return (1);
}
