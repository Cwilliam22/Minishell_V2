/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:26:27 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 11:26:42 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Nb_arg compte le nom d'argument avant expension
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
