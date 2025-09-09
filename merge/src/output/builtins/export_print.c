/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:44:41 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/05 16:10:15 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	count_env_variables(t_env *env_list)
{
	t_env	*current;
	int		count;

	current = env_list;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static t_env	**create_sorted_array(t_env *env_list)
{
	t_env	**sorted_array;
	t_env	*current;
	int		count;
	int		i;

	count = count_env_variables(env_list);
	if (count == 0)
		return (NULL);
	sorted_array = malloc(sizeof(t_env *) * (count + 1));
	if (!sorted_array)
		return (NULL);
	current = env_list;
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

static void	sort_env_array(t_env **array, int count)
{
	t_env	*temp;
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

static void	print_export_format(t_env *env)
{
	printf("declare -x %s", env->key);
	if (env->value)
		printf("=\"%s\"", env->value);
	printf("\n");
}

void	display_all_exports(t_shell *shell)
{
	t_env	**sorted_array;
	int		count;
	int		i;

	if (!shell->env_list)
		return ;
	sorted_array = create_sorted_array(shell->env_list);
	if (!sorted_array)
		return ;
	count = count_env_variables(shell->env_list);
	sort_env_array(sorted_array, count);
	i = 0;
	while (i < count)
	{
		print_export_format(sorted_array[i]);
		i++;
	}
	free(sorted_array);
}
