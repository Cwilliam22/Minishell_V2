/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:55:38 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/12 13:19:09 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_pipes_and_redir(int i, int **pipes, t_exec *exec)
{
	if (i == 0 && exec->nb_process > 1)
	{
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	else if (i == exec->nb_process - 1 && exec->nb_process > 1)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
	}
	else if (i > 0 && i < exec->nb_process - 1)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	close_pipes(pipes, exec);
}

static int	exec_pipe(t_exec *exec, int **pipes, pid_t *pids)
{
	int		i;
	t_cmd	*current_cmd;

	i = 0;
	current_cmd = exec->shell->commands;
	while (i < exec->nb_process && current_cmd)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			setup_child_pipes_and_redir(i, pipes, exec);
			exec->shell->commands = current_cmd;
			execute_single_command(exec);
			exit(exec->shell->env->last_exit_status);
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
					exit_status = 128 + WTERMSIG(status);
				else
					exit_status = EXIT_FAILURE;
			}
		}
		i++;
	}
	set_exit_status(exit_status);
}

void	handle_pipeline(t_exec *exec)
{
	int		**pipes;
	pid_t	*pids;

	if (!create_pipes(exec, &pipes))
	{
		print_error(NULL, NULL, "Error creating pipes\n");
		return ;
	}
	pids = (pid_t *)safe_malloc(sizeof(pid_t) * exec->nb_process);
	if (!exec_pipe(exec, pipes, pids))
	{
		free(pids);
		free_pipes(pipes, exec);
		return ;
	}
	close_pipes(pipes, exec);
	wait_all_child(exec, pids);
	free_pipes(pipes, exec);
	free(pids);
}
