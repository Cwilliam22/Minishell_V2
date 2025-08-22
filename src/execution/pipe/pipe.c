/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:58:18 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 15:37:47 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	manage_parent_fds(int *prev_read, int pipe_fds[2],
				int cmd_index, t_exec *exec)
{
	if (*prev_read != -1)
		close(*prev_read);
	if (cmd_index < exec->nb_process - 1)
	{
		close(pipe_fds[1]);
		*prev_read = pipe_fds[0];
	}
}

static void	setup_child_pipeline_fds(int prev_read, int pipe_fds[2],
				int cmd_index, t_exec *exec)
{
	if (cmd_index > 0)
	{
		dup2(prev_read, STDIN_FILENO);
		close(prev_read);
	}
	if (cmd_index < exec->nb_process - 1)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		close(pipe_fds[0]);
	}
}

static int	create_pipe_if_needed(int cmd_index, t_exec *exec, int pipe_fds[2])
{
	if (cmd_index < exec->nb_process - 1)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}

static pid_t	create_pipeline_process(t_cmd *cmd, t_exec *exec,
					int *prev_read, int cmd_index)
{
	pid_t	pid;
	int		pipe_fds[2];

	if (create_pipe_if_needed(cmd_index, exec, pipe_fds) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		setup_child_pipeline_fds(*prev_read, pipe_fds, cmd_index, exec);
		child_process(cmd, exec);
	}
	else if (pid > 0)
		manage_parent_fds(prev_read, pipe_fds, cmd_index, exec);
	else
		perror("fork");
	return (pid);
}

void	handle_pipeline(t_cmd *cmds, t_exec *exec)
{
	int		prev_read;
	int		i;

	exec->pids = NULL;
	prev_read = -1;
	i = 0;
	sig_core_dump_parent_signal();
	while (cmds && i < exec->nb_process)
	{
		if (i == 0)
		{
			exec->pids = malloc(sizeof(pid_t) * exec->nb_process);
			if (!exec->pids)
				return ;
		}
		exec->pids[i] = create_pipeline_process(cmds, exec, &prev_read, i);
		if (exec->pids[i] == -1)
		{
			free(exec->pids);
			exec->pids = NULL;
			return ;
		}
		cmds = cmds->next;
		i++;
	}
	wait_all_pipeline_children(exec->pids, exec->nb_process);
	free(exec->pids);
	exec->pids = NULL;
	parent_signal();
}
