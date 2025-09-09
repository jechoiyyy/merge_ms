/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:57:14 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/09 13:49:32 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	create_pipes(int **pipe_fds, int cmd_count)
{
	int	i;

	if (cmd_count <= 1)
	{
		*pipe_fds = NULL;
		return (SUCCESS);
	}
	*pipe_fds = malloc(sizeof(int) * 2 * (cmd_count - 1));
	if (!*pipe_fds)
		return (FAILURE);
	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipe(&(*pipe_fds)[i * 2]) == -1)
		{
			perror("pipe");
			while (--i >= 0)
			{
				close((*pipe_fds)[i * 2]);
				close((*pipe_fds)[i * 2 + 1]);
			}
			free(*pipe_fds);
			*pipe_fds = NULL;
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

// static void	close_all_pipes_local(int *pipe_fds, int cmd_count)
// {
// 	int	i;

// 	if (!pipe_fds || cmd_count <= 1)
// 		return ;
// 	i = 0;
// 	while (i < cmd_count - 1)
// 	{
// 		close(pipe_fds[i * 2]);
// 		close(pipe_fds[i * 2 + 1]);
// 		i++;
// 	}
// }

static int	fork_and_execute(t_cmd *cmd, t_shell *shell, int *pipe_fds, 
							int cmd_index, int cmd_count)
{
	pid_t	pid;

	pid = fork_process();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		// 자식 프로세스
		setup_child_process(cmd, pipe_fds, cmd_index, cmd_count);
		exit(execute_command(cmd, shell));
	}
	else
	{
		// 부모 프로세스
		setup_parent_process(pipe_fds, cmd_index, cmd_count);
	}
	return (pid);
}

static void	cleanup_resources(int *pipe_fds, pid_t *pids, int cmd_count)
{
	if (pipe_fds)
	{
		close_all_pipes(pipe_fds, cmd_count - 1);  // 헤더의 함수 사용
		free(pipe_fds);
	}
	if (pids)
		free(pids);
}

static int	handle_single_builtin(t_cmd *commands, t_shell *shell)
{
	int	result;

	// 단일 빌트인 명령어는 현재 쉘에서 직접 실행
	result = execute_builtin(commands, shell);
	shell->last_exit_status = result;
	return (result);
}

int	execute_pipeline(t_cmd *commands, t_shell *shell)
{
	int		*pipe_fds;
	pid_t	*pids;
	t_cmd	*current;
	int		cmd_count;
	int		i;

	if (!commands)
		return (FAILURE);
	
	cmd_count = count_commands(commands);
	
	// 단일 빌트인 명령어 처리
	if (cmd_count == 1 && is_builtin_command(commands->args[0]))
		return (handle_single_builtin(commands, shell));
	
	// 파이프 생성
	if (create_pipes(&pipe_fds, cmd_count) == FAILURE)
		return (FAILURE);
	
	// PID 배열 할당
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		if (pipe_fds)
		{
			close_all_pipes(pipe_fds, cmd_count - 1);  // 헤더의 함수 사용
			free(pipe_fds);
		}
		return (FAILURE);
	}
	
	// 각 명령어를 포크하여 실행
	current = commands;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork_and_execute(current, shell, pipe_fds, i, cmd_count);
		if (pids[i] == -1)
		{
			// 포크 실패 시 이미 생성된 자식 프로세스들을 종료
			while (--i >= 0)
				kill(pids[i], SIGTERM);
			cleanup_resources(pipe_fds, pids, cmd_count);
			return (FAILURE);
		}
		current = current->next;
		i++;
	}
	
	// 부모 프로세스에서 모든 파이프 닫기
	close_all_pipes(pipe_fds, cmd_count - 1);  // 헤더의 함수 사용
	
	// 모든 자식 프로세스 종료 대기
	shell->last_exit_status = wait_for_children(pids, cmd_count);
	
	// 리소스 정리
	cleanup_resources(pipe_fds, pids, cmd_count);
	
	return (SUCCESS);
}
