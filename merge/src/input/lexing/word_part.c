/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_part.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:03:51 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 10:11:58 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "utils.h"

static char	*skip_quotes(char *s)
{
	char	qc;

	qc = *s;
	s++;
	while (*s && *s != qc)
		s++;
	if (*s == qc)
		return (++s);
	return (s);
}

static int	get_word_length(char *input)
{
	char	*start;

	start = input;
	while (*input && !is_space(*input) && !is_operator(*input))
	{
		if (*input == '\'' || *input == '"')
			input = skip_quotes(input);
		else
			input++;
	}
	return (input - start);
}

int	handle_word(t_token **token_lst, char *input)
{
	int				len;
	char			*value;

	len = get_word_length(input);
	if (len == 0)
		return (-1);
	value = ft_my_strndup(input, len);
	if (!value)
		return (-1);
	if (add_new_token(token_lst, value, T_WORD))
		return (len);
	return (-1);
}
