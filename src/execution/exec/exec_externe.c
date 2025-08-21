/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_externe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:08:30 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 16:09:04 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_cmd *cmd, t_exec *exec)
{
	exec->env_copy = env_to_string(exec->shell->env);
	if (access(cmd->cmd_path, X_OK) == 0)
	{
		execve(cmd->cmd_path, cmd->args_expanded,
			exec->env_copy);
		print_error(cmd->cmd_path, NULL, "execve failed");
	}
	else
	{
		print_error(cmd->cmd_path, cmd->args_expanded[1],
			"Permission denied");
	}
}

static int	parent_process(pid_t pid)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}

int	execute_externe(t_cmd *cmd, t_exec *exec)
{
	pid_t	pid;
	int		result;

	exec->current_cmd = cmd;
	update_state_path(cmd);
	if (!apply_cmd_path(cmd, exec))
		return (exec->shell->env->last_exit_status);
	sig_core_dump_parent_signal();
	pid = fork();
	if (pid == 0)
	{
		child_signal();
		child_process(cmd, exec);
		exit(COMMAND_NOT_EXECUTABLE);
	}
	else if (pid > 0)
	{
		result = parent_process(pid);
		parent_signal();
		return (result);
	}
	else
		return (parent_signal(), print_error(NULL, NULL, "Fork failed"), 1);
}
