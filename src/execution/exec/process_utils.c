/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:23:18 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:23:25 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates exit status from child process
 * @param status Status returned by waitpid()
 */
void	update_exit_status_from_child(int status)
{
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(128 + WTERMSIG(status));
	else
		set_exit_status(1);
}

static void	execute_externe(t_cmd *cmd, t_exec *exec)
{
	int	exit_code;

	exec->current_cmd = cmd;
	update_state_path(cmd);
	if (!apply_cmd_path(cmd, exec))
	{
		exit_code = get_exit_status(exec);
		cleanup_and_exit(exit_code, exec);
	}
	if (access(cmd->cmd_path, X_OK) != 0)
	{
		print_error(cmd->args_expanded[0], NULL, "Permission denied");
		cleanup_and_exit(126, exec);
	}
	execve(cmd->cmd_path, cmd->args_expanded, exec->env_copy);
	perror(cmd->args_expanded[0]);
	cleanup_and_exit(126, exec);
}

/**
 * @brief Handles execution in child process
 * @param cmd Command to execute
 * @param exec Execution context
 */
void	child_process(t_cmd *cmd, t_exec *exec)
{
	int	exit_status;

	exit_status = 0;
	child_signal();
	if (apply_redirections(cmd) != 0)
		cleanup_and_exit(1, exec);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, exec);
		exit_status = get_exit_status(exec);
		cleanup_and_exit(exit_status, exec);
	}
	else
		execute_externe(cmd, exec);
}
