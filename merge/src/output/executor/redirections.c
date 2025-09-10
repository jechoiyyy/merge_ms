/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:00:39 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 14:23:31 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	setup_redirections(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	if (!cmd)
		return (FAILURE);
	fd_in = -1;
	fd_out = -1;
	if (cmd->input_file && cmd->input_file->filename && 
		strcmp(cmd->input_file->filename, "NULL") != 0)
	{
		if (cmd->input_file->flag == 1)
			return (print_error("export값", "ambiguous redirect"), FAILURE);
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
	if (cmd->output_file && cmd->output_file->filename && 
		strcmp(cmd->output_file->filename, "NULL") != 0)
	{
		if (cmd->output_file->flag == 1)
			return (print_error("export값", "ambiguous redirect"), FAILURE);
		fd_out = open_output_file(cmd->output_file->filename, cmd->append_mode);
		if (fd_out == -1)
			return (FAILURE);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			close(fd_out);
			return (FAILURE);
		}
		close(fd_out);
	}
	if (cmd->hd && cmd->hd != -1)
	{
		fd_in = cmd->hd;
		if (fd_in == -1)
			return (FAILURE);
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			close(fd_in);
			return (FAILURE);
		}
		close(fd_in);
	}
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

// int	setup_heredoc(int fd)
// {
// 	int		pipe_fd[2];
// 	char	*line;
// 	char	buffer[1024];

// 	if (!delimiter)
// 		return (-1);
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	printf("heredoc> ");
// 	while (fgets(buffer, sizeof(buffer), stdin))
// 	{
// 		line = buffer;
// 		while (*line && (*line == ' ' || *line == '\t'))
// 			line++;
// 		if (strncmp(line, delimiter, strlen(delimiter)) == 0
// 			&& (line[strlen(delimiter)] == '\n' || line[strlen(delimiter)] == '\0'))
// 			break ;
// 		write(pipe_fd[WRITE_END], buffer, strlen(buffer));
// 		printf("heredoc> ");
// 	}
// 	close(pipe_fd[WRITE_END]);
// 	return (pipe_fd[READ_END]);
// }