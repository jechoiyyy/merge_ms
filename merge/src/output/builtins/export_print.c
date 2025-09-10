/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:44:41 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 13:38:55 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	count_env_variables(t_envp *envp_list)
{
	t_envp	*current;
	int		count;

	current = envp_list;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static t_envp	**create_sorted_array(t_envp *envp_list)
{
	t_envp	**sorted_array;
	t_envp	*current;
	int		count;
	int		i;

	count = count_env_variables(envp_list);
	if (count == 0)
		return (NULL);
	sorted_array = malloc(sizeof(t_envp *) * (count + 1));
	if (!sorted_array)
		return (NULL);
	current = envp_list;
	i = 0;
	while (current)
	{
		sorted_array[i] = current;
		current = current->next;
		i++;
	}
	sorted_array[count] = NULL;
	return (sorted_array);
}

static void	sort_env_array(t_envp **array, int count)
{
	t_envp	*temp;
	int		i;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strcmp(array[j]->key, array[j + 1]->key) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_format(t_envp *env)
{
	printf("declare -x %s", env->key);
	if (env->value)
		printf("=\"%s\"", env->value);
	printf("\n");
}

void	display_all_exports(t_shell *shell)
{
	t_envp	**sorted_array;
	int		count;
	int		i;

	if (!shell->envp_list)
		return ;
	sorted_array = create_sorted_array(shell->envp_list);
	if (!sorted_array)
		return ;
	count = count_env_variables(shell->envp_list);
	sort_env_array(sorted_array, count);
	i = 0;
	while (i < count)
	{
		print_export_format(sorted_array[i]);
		i++;
	}
	free(sorted_array);
}
