/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 11:01:35 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/09 19:23:16 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	set_input_file(t_filename *file, t_token *current)
{
	if (!file || !current)
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

void	set_output_file(t_cmd *cmd, t_filename *file, t_token *current, int append)
{
	if (!file || !current)
		return ;
	if (file->filename)
		free(file->filename);
	file->filename = malloc(ft_strlen(current->value) + 1);
	if (!file->filename)
		return ;
	ft_strcpy(file->filename, current->value);
	cmd->append_mode = append;
	if (current->type == T_WRONG_FILNAME)
		file->flag = 1;
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