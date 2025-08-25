/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:42:00 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:42:00 by alfavre          ###   ########.ch       */
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

pid_t	create_pipeline_process(t_cmd *cmd, t_exec *exec,
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
