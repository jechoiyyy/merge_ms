/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:00:39 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/12 05:18:56 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <stdio.h>

int	setup_redirections(t_cmd *cmd, int cmd_index, int cmd_count)
{
	int	fd_in;
	int	fd_out;
	int	heredoc_fd;
	t_filename *file;

	(void)cmd_index;
	(void)cmd_count;
	if (!cmd)
		return (FAILURE);
	fd_in = -1;
	fd_out = -1;
	heredoc_fd = -1;

	// heredoc 처리 - fd는 나중에 닫기
	if (cmd->hd && cmd->hd != -1)
	{
		heredoc_fd = cmd->hd;
		if (dup2(cmd->hd, STDIN_FILENO) == -1)
			return (close(cmd->hd), FAILURE);
	}
	// 입력 리다이렉션 처리 (stdin)
	if (cmd->input_file && cmd->input_file->filename && 
		ft_strcmp(cmd->input_file->filename, "NULL") != 0)
	{
		if (cmd->input_file->flag == 1)
			return (print_error(cmd->input_file->filename, "ambiguous redirect"), FAILURE);
		fd_in = open_input_file(cmd->input_file->filename);
		if (fd_in == -1)
			return (FAILURE);
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			close(fd_in);
			return (FAILURE);
		}
		close(fd_in);
	}
	
	// 출력 리다이렉션 처리 (stdout) - 파이프가 있는 중간 명령어에서는 출력 리다이렉션이 파이프를 덮어씀
	if (cmd->output_file && cmd->output_file->filename && 
		ft_strcmp(cmd->output_file->filename, "NULL") != 0)
	{
		file = cmd->output_file;
		while (file)
		{
			if (file->flag == 1)
				return (print_error(file->filename, "ambiguous redirect"), FAILURE);
			fd_out = open_output_file(file->filename, file->append_mode);
			if (fd_out == -1)
				return (FAILURE);
			if (file->next == NULL)
			{
				if (dup2(fd_out, STDOUT_FILENO) == -1)
				{
						close(fd_out);
						return (FAILURE);
				}
				close(fd_out);
			}
			file = file->next;
		}
	}
	// heredoc fd는 마지막에 닫기
	if (heredoc_fd != -1)
		close(heredoc_fd);
		
	return (SUCCESS);
}

int	open_input_file(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_error(filename, strerror(errno));
		return (-1);
	}
	return (fd);
}

int	open_output_file(char *filename, int append_mode)
{
	int	fd;
	int	flags;

	if (!filename)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		print_error(filename, strerror(errno));
		return (-1);
	}
	return (fd);
}
