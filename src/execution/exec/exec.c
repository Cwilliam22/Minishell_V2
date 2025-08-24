/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:28:55 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 11:30:06 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipeline_or_simple(t_exec *exec, t_cmd *cmd)
{
	if (exec->is_pipe)
		handle_pipeline(cmd, exec);
	else
	{
		if (check_args(exec))
			execute_single_command(cmd, exec);
		else
			set_exit_status(127);
	}
}

void	execute_commands(t_shell *shell)
{
	t_exec	*exec;
	t_cmd	*cmds;

	if (!shell || !shell->commands)
		return ;
	exec = create_exec(shell);
	if (!exec)
		return ;
	cmds = shell->commands;
	if (exec->nb_process == 0 || !cmds->args_expanded)
	{
		if (cmds->redirections)
			handle_redirection_only(cmds, exec);
		free_exec(exec);
		return ;
	}
	pipeline_or_simple(exec, cmds);
	free_exec(exec);
}
