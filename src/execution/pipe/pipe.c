/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:57:42 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 16:06:55 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_pipes_and_redir(int i, int **pipes, t_exec *exec)
{
	if (i == 0 && exec->nb_process > 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	else if (i == exec->nb_process - 1 && exec->nb_process > 1)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	else if (i > 0 && i < exec->nb_process - 1)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	close_pipes(pipes, exec);
}

static void	child_process(t_exec *exec, int **pipes, pid_t *pids, int i)
{
	int		exit_status;

	setup_child_pipes_and_redir(i, pipes, exec);
	execute_single_command(exec->current_cmd, exec);
	free_pipes(pipes, exec);
	free(pids);
	exit_status = exec->shell->env->last_exit_status;
	cleanup_all(exec);
	free(exec);
	exit(exit_status);
}

static int	exec_pipe(t_cmd *cmd, t_exec *exec, int **pipes, pid_t *pids)
{
	int		i;
	t_cmd	*current_cmd;

	i = 0;
	current_cmd = cmd;
	while (i < exec->nb_process && current_cmd)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			exec->current_cmd = current_cmd;
			child_signal();
			child_process(exec, pipes, pids, i);
		}
		else if (pids[i] < 0)
		{
			kill_all_process(pids, i);
			return (print_error(NULL, NULL, "Error in fork\n"), 0);
		}
		current_cmd = current_cmd->next;
		i++;
	}
	return (1);
}

void	wait_all_child(t_exec *exec, pid_t *pids)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	while (i < exec->nb_process)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (i == exec->nb_process - 1)
			{
				if (WIFEXITED(status))
					exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
				{
					int sig = WTERMSIG(status);
					exit_status = 128 + sig;
					if (sig == SIGINT)
						write(STDERR_FILENO, "\n", 1);
					else if (sig == SIGQUIT)
						write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				}
				else
					exit_status = EXIT_FAILURE;
			}
		}
		i++;
	}
	set_exit_status(exit_status);
}

void	handle_pipeline(t_cmd *cmd, t_exec *exec)
{
	int		**pipes;
	pid_t	*pids;

	if (!create_pipes(exec, &pipes))
	{
		print_error(NULL, NULL, "Error creating pipes\n");
		return ;
	}
	pids = (pid_t *)safe_malloc(sizeof(pid_t) * exec->nb_process);
	sig_core_dump_parent_signal();
	if (!exec_pipe(cmd, exec, pipes, pids))
	{
		free(pids);
		free_pipes(pipes, exec);
		return ;
	}
	close_pipes(pipes, exec);
	wait_all_child(exec, pids);
	free_pipes(pipes, exec);
	free(pids);
	parent_signal();
}
