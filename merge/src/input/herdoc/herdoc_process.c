/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:59:44 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 17:38:22 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "list.h"
#include "libft.h"
#include "expand.h"
#include "global.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#define DELIM_PRINT "minishell: warning: here-document \
delimited by end-of-file (wanted `%s')\n"

typedef struct s_delim_info
{
	char	*cooked_delim;
	int		quoted_flag;
}	t_delim_info;

static int	detect_quotes(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
}

static size_t	get_len_without_quotes(char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		if (*str != '\'' && *str != '"')
			len++;
		str++;
	}
	return (len);
}

static void	copy_without_quotes(char *dest, char *src)
{
	while (*src)
	{
		if (*src != '\'' && *src != '"')
			*dest++ = *src;
		src++;
	}
	*dest = '\0';
}

t_delim_info	make_delim(char *raw)
{
	t_delim_info	info;

	info.cooked_delim = malloc(get_len_without_quotes(raw) + 1);
	if (!info.cooked_delim)
		return (info);
	info.quoted_flag = detect_quotes(raw);
	copy_without_quotes(info.cooked_delim, raw);
	return (info);
}

static void	heredoc_child(char *delim, int write_fd, t_envp *envp)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line && printf(DELIM_PRINT, delim))
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_process(line, envp);
		if (!expanded)
			exit(1);
		free(line);
		ft_putendl_fd(expanded, write_fd);
		free(expanded);
	}
	close(write_fd);
	exit(0);
}

//전역변수 exit_status 계산법이라는데 gpt한테 받은거라 아직 잘 모릅니다...
//나중에 알게되면 알려드를게요
//기본적으로 자식 프로세스 생성해서 입력을 받습니다.
int	read_heredoc(t_prompt *prompt, t_delim_info delim_info)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) < 0)
		return (-2);
	pid = fork();
	if (pid < 0)
		return (-2);
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_child(delim_info.cooked_delim, fd[1], prompt->envp_lst);
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	if (g_exit_status != 0)
		return (close(fd[0]), -1);
	return (fd[0]);
}

//herdoc value 처리해서 delim 만들어서 사용
//자식프로세스 에서 fd 1에 입력후 fd 0반환
int	heredoc_process(t_prompt *prompt)
{
	t_token			*curr;
	t_delim_info	delim_info;
	int				fd;

	curr = prompt->token_lst;
	while (curr && curr->type != T_ERROR)
	{
		if (curr->type == T_HEREDOC && curr->next && curr->next->type == T_WORD)
		{
			delim_info = make_delim(curr->next->value);
			if (!delim_info.cooked_delim)
				return (0);
			fd = read_heredoc(prompt, delim_info);
			free(delim_info.cooked_delim);
			if (fd == -1)
				return (clear_token_list(&prompt->token_lst), 1);
			if (fd == -2 || !add_new_hd(&prompt->hd_lst, fd))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}
