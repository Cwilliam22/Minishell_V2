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
	if (exec->nb_process == 1)
	{
		if (check_args(exec))
			execute_single_command(cmd, exec);
	}
	else if (exec->nb_process > 1)
		handle_pipeline(cmd, exec);
}

void	execute_commands(t_shell *shell)
{
	t_exec	*exec;
	t_cmd	*cmds;

	exec = create_exec(shell);
	if (!exec)
		return ;
	update_var_path(exec);
	cmds = shell->commands;
	if (!cmds || !cmds->args_expanded
		|| !cmds->args_expanded[0])
	{
		if (cmds->redirections)
			handle_redirection_only(cmds);
	}
	else
		pipeline_or_simple(exec, cmds);
	free_exec(exec);
	free(exec);
}
