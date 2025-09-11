/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:57:14 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 17:21:20 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	create_pipes(int **pipe_fds, int cmd_count)
{
	int	i;
	int	j;

	if (cmd_count <= 1)
	{
		*pipe_fds = NULL;
		return (SUCCESS);
	}
	*pipe_fds = malloc(sizeof(int) * 2 * (cmd_count - 1));
	if (!*pipe_fds)
		return (FAILURE);
	
	// 모든 FD를 -1로 초기화 (안전을 위해)
	i = 0;
	while (i < (cmd_count - 1) * 2)
	{
		(*pipe_fds)[i] = -1;
		i++;
	}
	
	// 파이프 생성
	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipe(&(*pipe_fds)[i * 2]) == -1)
		{
			perror("pipe");
			// 이미 생성된 파이프들 정리
			j = 0;
			while (j < i)
			{
				if ((*pipe_fds)[j * 2] >= 0)
					close((*pipe_fds)[j * 2]);
				if ((*pipe_fds)[j * 2 + 1] >= 0)
					close((*pipe_fds)[j * 2 + 1]);
				j++;
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
	int		exit_code;

	pid = fork_process(); 
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		// 자식 프로세스: 순서 중요!
		// 1. 파이프 설정 및 모든 파이프 FD 닫기
		setup_child_process(cmd, pipe_fds, cmd_index, cmd_count);
		
		// 2. 리다이렉션 설정 (파이프 설정 후에)
		if (setup_redirections(cmd) == FAILURE)
			exit(1);
		
		// 3. 명령어 유효성 검사
		if (!cmd || !cmd->args || !cmd->args[0])
			exit(1);
		
		// 4. 명령어 실행
		if (is_builtin_command(cmd->args[0]))
			exit_code = execute_builtin(cmd, shell);
		else
			exit_code = execute_external(cmd, shell);
		
		exit(exit_code);
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

	if (!commands || !shell)
		return (FAILURE);
	
	cmd_count = count_commands(commands);

	if (cmd_count == 1 && commands->args && is_builtin_command(commands->args[0]))
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
