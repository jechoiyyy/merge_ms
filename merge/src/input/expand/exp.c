/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:14:02 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/05 17:52:13 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "global.h"
#include "expand.h"

static char	*get_expanded_value(char *p, size_t len, t_envp *envp_lst)
{
	char	*expanded;

	expanded = NULL;
	while (envp_lst)
	{
		if (ft_strncmp(envp_lst->key, p, len) == 0 \
			&& envp_lst->key[len] == '\0')
		{
			expanded = ft_strdup(envp_lst->value);
			return (expanded);
		}
		envp_lst = envp_lst->next;
	}
	return (ft_strdup(""));
}

static char	*expand_env(char *p, t_envp *envp_lst, size_t *idx)
{
	size_t	start;
	size_t	len;
	char	*expanded;

	start = *idx;
	len = 0;
	if (p[start] == '?')
	{
		*idx += 1;
		return (ft_itoa(g_exit_status));
	}
	while (p[start + len] && \
			(ft_isalnum(p[start + len]) || p[start + len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	expanded = get_expanded_value(p + start, len, envp_lst);
	*idx += len;
	return (expanded);
}

int	exp_normal_process(t_exp *exp_data, char *value)
{
	char	*new_result;

	new_result = charjoin_and_free(exp_data->result, value[exp_data->idx]);
	if (!new_result)
	{
		perror("malloc fail : charjoin_and_free");
		exp_data->result = NULL;
		return (0);
	}
	exp_data->result = new_result;
	return (1);
}

int	exp_env_process(t_exp *exp_data, char *value, t_envp *envp_lst)
{
	char	*expanded;
	char	*temp;

	exp_data->idx++;
	expanded = expand_env(value, envp_lst, &exp_data->idx);
	if (!expanded)
	{
		perror("malloc fail : expand_env");
		return (free(exp_data->result), exp_data->result = NULL, 0);
	}
	temp = ft_strjoin(exp_data->result, expanded);
	if (!temp)
	{
		perror("malloc fail : ft_strjoin");
		return (free(exp_data->result), exp_data->result = NULL, \
				free(expanded), 0);
	}
	free(exp_data->result);
	free(expanded);
	exp_data->result = temp;
	return (1);
}

int	set_exp_data(t_exp *exp_data)
{
	char	*res;

	res = ft_strdup("");
	if (!res)
	{
		perror("malloc fail : set_exp_data");
		return (0);
	}
	exp_data->state = STATE_GENERAL;
	exp_data->idx = 0;
	exp_data->result = res;
	return (1);
}
