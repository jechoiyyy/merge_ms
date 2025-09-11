/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:44:04 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/12 03:32:27 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

t_envp	*create_envp(char *title, char *value)
{
	t_envp	*new_envp;

	new_envp = (t_envp *)malloc(sizeof(t_envp));
	if (!new_envp)
	{
		perror("malloc fail : create_envp");
		return (NULL);
	}
	new_envp->key = title;
	new_envp->value = value;
	new_envp->next = NULL;
	return (new_envp);
}

t_envp	*find_last_envp(t_envp *envp_list)
{
	if (!envp_list)
		return (NULL);
	while (envp_list->next)
		envp_list = envp_list->next;
	return (envp_list);
}

void	addback_envp(t_envp **envp_list, t_envp *new)
{
	t_envp	*last_envp;

	if (!envp_list || !new)
		return ;
	if (*envp_list == NULL)
	{
		*envp_list = new;
		return ;
	}
	last_envp = find_last_envp(*envp_list);
	last_envp->next = new;
}

void	clear_envp_list(t_envp **envp_list)
{
	t_envp	*curr;
	t_envp	*next;

	if (!envp_list || !*envp_list)
		return ;
	curr = *envp_list;
	while (curr)
	{
		next = curr->next;
		free(curr->key);
		curr->key = NULL;
		free(curr->value);
		curr->value = NULL;
		free(curr);
		curr = next;
	}
	*envp_list = NULL;
}
