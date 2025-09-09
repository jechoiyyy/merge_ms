/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:43:17 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/05 17:40:15 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "setup.h"
#include "stdio.h"
#include "list.h"

static char	*extract_title(char *env_str)
{
	char	*equal_pos;
	char	*title;
	size_t	len;

	equal_pos = ft_strchr(env_str, '=');
	if (!equal_pos)
		return (NULL);
	len = equal_pos - env_str;
	title = ft_my_strndup(env_str, len);
	if (!title)
	{
		perror("malloc fail : extract_title");
		return (NULL);
	}
	return (title);
}

static char	*extract_value(char *env_str)
{
	char	*equal_pos;
	char	*value;

	equal_pos = ft_strchr(env_str, '=');
	if (!equal_pos)
		return (NULL);
	value = ft_strdup(equal_pos + 1);
	if (!value)
	{
		perror("malloc fail : extract_value");
		return (NULL);
	}
	return (value);
}

t_envp	*set_envp(char **envp)
{
	t_envp	*envp_lst;
	t_envp	*new_envp;
	char	*title;
	char	*value;

	envp_lst = NULL;
	while (*envp)
	{
		title = extract_title(*envp);
		value = extract_value(*envp);
		if (!title || !value)
			return (free(title), free(value), clear_envp_list(&envp_lst), NULL);
		new_envp = create_envp(title, value);
		if (!new_envp)
			return (clear_envp_list(&envp_lst), NULL);
		addback_envp(&envp_lst, new_envp);
		envp++;
	}
	return (envp_lst);
}
