/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 09:52:37 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/12 03:30:36 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->value = value;
	new_token->type = type;
	return (new_token);
}

t_token	*find_last_token(t_token *token_list)
{
	if (!token_list)
		return (NULL);
	while (token_list->next)
		token_list = token_list->next;
	return (token_list);
}

void	addback_token(t_token **token_list, t_token *new)
{
	t_token	*last_token;

	if (!token_list || !new)
		return ;
	if (*token_list == NULL)
	{
		*token_list = new;
		return ;
	}
	last_token = find_last_token(*token_list);
	last_token->next = new;
}

void	clear_token_list(t_token **token_list)
{
	t_token	*curr;
	t_token	*next;

	if (!token_list || !*token_list)
		return ;
	curr = *token_list;
	while (curr)
	{
		next = curr->next;
		free(curr->value);
		curr->value = NULL;
		free(curr);
		curr = next;
	}
	*token_list = NULL;
}

int	add_new_token(t_token **token_lst, char *value, t_token_type type)
{
	t_token	*new_node;

	new_node = create_token(value, type);
	if (!new_node)
	{
		free(value);
		return (0);
	}
	addback_token(token_lst, new_node);
	return (1);
}
