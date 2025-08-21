/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:01:27 by alexis            #+#    #+#             */
/*   Updated: 2025/08/21 23:50:27 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	exec->current_cmd = cmd;
	update_state_path(cmd);
	if (!apply_cmd_path(cmd, exec))
		exit(get_exit_status(exec));
	if (access(cmd->cmd_path, X_OK) != 0)
	{
		print_error(cmd->args_expanded[0], NULL, "Permission denied");
		exit(126);
	}
	execve(cmd->cmd_path, cmd->args_expanded, exec->env_copy);
	perror(cmd->args_expanded[0]);
	exit(126);
}

void	child_process(t_cmd *cmd, t_exec *exec)
{
	if (apply_redirections(cmd) != 0)
		exit(1);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, exec);
		exit(get_exit_status(exec));
	}
	else
		execute_externe(cmd, exec);
}
