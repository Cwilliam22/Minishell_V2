/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:41:28 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:41:51 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_all_pipeline_children(pid_t *pids, int nb_process)
{
	int	i;
	int	status;
	int	last_exit_code;

	last_exit_code = 0;
	i = 0;
	while (i < nb_process)
	{
		waitpid(pids[i], &status, 0);
		if (i == nb_process - 1)
		{
			if (WIFEXITED(status))
				last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
	set_exit_status(last_exit_code);
}

static int	init_pids_array(t_exec *exec)
{
	exec->pids = malloc(sizeof(pid_t) * exec->nb_process);
	if (!exec->pids)
		return (0);
	return (1);
}

static void	cleanup_pipeline(t_exec *exec)
{
	if (exec->pids)
	{
		free(exec->pids);
		exec->pids = NULL;
	}
}

static int	process_pipeline_commands(t_cmd *cmds, t_exec *exec)
{
	int		prev_read;
	int		i;
	t_cmd	*current;

	prev_read = -1;
	i = 0;
	current = cmds;
	while (current && i < exec->nb_process)
	{
		if (i == 0 && !init_pids_array(exec))
			return (0);
		exec->pids[i] = create_pipeline_process(current, exec, &prev_read, i);
		if (exec->pids[i] == -1)
			return (0);
		current = current->next;
		i++;
	}
	return (1);
}

void	handle_pipeline(t_cmd *cmds, t_exec *exec)
{
	exec->pids = NULL;
	sig_core_dump_parent_signal();
	if (!process_pipeline_commands(cmds, exec))
	{
		cleanup_pipeline(exec);
		parent_signal();
		return ;
	}
	wait_all_pipeline_children(exec->pids, exec->nb_process);
	cleanup_pipeline(exec);
	parent_signal();
}
