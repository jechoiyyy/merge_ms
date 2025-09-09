/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 11:01:35 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/09 21:21:10 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_filename	*create_filename(void)
{
	t_filename	*new;

	new = malloc(sizeof(t_filename));
	if (!new)
		return (NULL);
	new->filename = NULL;
	new->flag = 0;
	new->next = NULL;

	return (new);
}

void	set_input_file(t_filename *file, t_token *current)
{
	if (!file || !current || !current->value)
		return ;
	if (file->filename)
		free(file->filename);
	file->filename = malloc(ft_strlen(current->value) + 1);
	if (!file->filename)
		return ;
	ft_strcpy(file->filename, current->value);
	if (current->type == T_WRONG_FILNAME)
		file->flag = 1;
}

static void	add_filename_to_list(t_filename **head, t_filename *new_node)
{
	t_filename	*current;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	set_output_file(t_cmd *cmd, t_token *current, int append)
{
	t_filename	*new_file;

	if (!cmd || !current || !current->value)
		return ;
	new_file = create_filename();
	new_file->filename = malloc(ft_strlen(current->value) + 1);
	if (!new_file)
		return ;
	ft_strcpy(new_file->filename, current->value);
	if (current->type == T_WRONG_FILNAME)
		new_file->flag = 1;
	add_filename_to_list(&(cmd->output_file), new_file);
	cmd->append_mode = append;
}

// void	set_heredoc_delimiter(t_cmd *cmd, char *delimiter)
// {
// 	if (!cmd || !delimiter)
// 		return ;
// 	if (cmd->heredoc_delimiter)
// 		free(cmd->heredoc_delimiter);
// 	cmd->heredoc_delimiter = malloc(ft_strlen(delimiter) + 1);
// 	if (!cmd->heredoc_delimiter)
// 		return ;
// 	ft_strcpy(cmd->heredoc_delimiter, delimiter);
// }