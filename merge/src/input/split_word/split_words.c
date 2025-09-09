/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:42:50 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 17:15:07 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "lexing.h"
#include "list.h"
#include "libft.h"
#include "utils.h"

static int	only_space(char *value)
{
	if (!value)
		return (1);
	while (*value)
	{
		if (!is_space(*value))
			return (0);
		value++;
	}
	return (1);
}

static int	process_word_token(t_token *curr, t_token **new_lst)
{
	t_token	*lexed;
	t_token	*tmp;

	if (only_space(curr->value))
		return (1);
	lexed = input_lexing(curr->value, 0);
	if (!lexed)
		return (0);
	tmp = lexed;
	while (tmp)
	{
		if (!add_new_token(new_lst, ft_strdup(tmp->value), tmp->type))
		{
			clear_token_list(&lexed);
			clear_token_list(new_lst);
			return (0);
		}
		tmp = tmp->next;
	}
	clear_token_list(&lexed);
	return (1);
}

static int	process_other_token(t_token *curr, t_token **new_lst)
{
	if (!add_new_token(new_lst, ft_strdup(curr->value), curr->type))
		return (0);
	return (1);
}

int	split_words(t_token **token_lst)
{
	t_token	*curr;
	t_token	*new_lst;

	if (!token_lst)
		return (0);
	curr = *token_lst;
	new_lst = NULL;
	while (curr && curr->type != T_END)
	{
		if (curr->type == T_WORD)
		{
			if (!process_word_token(curr, &new_lst))
				return (0);
		}
		else
		{
			if (!process_other_token(curr, &new_lst))
				return (0);
		}
		curr = curr->next;
	}
	if (!add_new_token(&new_lst, ft_strdup("newline"), T_END))
		return (0);
	return (clear_token_list(token_lst), *token_lst = new_lst, 1);
}
