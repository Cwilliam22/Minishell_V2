/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:22:45 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:22:45 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a single command
 * @param cmd Command structure to execute
 * @param exec Execution context structure
 */
void	execute_single_command(t_cmd *cmd, t_exec *exec)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd) && !cmd->redirections)
	{
		execute_builtin(cmd, exec);
		return ;
	}
	sig_core_dump_parent_signal();
	pid = fork();
	if (pid == 0)
	{
		child_process(cmd, exec);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		update_exit_status_from_child(status);
	}
	else
	{
		perror("fork");
		set_exit_status(1);
	}
	parent_signal();
}
