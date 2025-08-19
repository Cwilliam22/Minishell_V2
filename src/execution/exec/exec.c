/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:37:01 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/19 18:38:24 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_shell *shell)
{
	t_exec	*exec;
	t_cmd	*cmd;

	exec = create_exec(shell);
	if (!exec)
		return ;
	update_var_path(exec);
	if (!shell->commands || !shell->commands->args_expanded
		|| !shell->commands->args_expanded[0])
	{
		if(shell->commands->redirections)
			handle_redirection_only(shell->commands);
	}
	else
	{
		cmd = shell->commands;
		if (exec->nb_process == 1)
		{
			if (check_args(exec))
				execute_single_command(cmd, exec);
		}
		else if (exec->nb_process > 1)
			handle_pipeline(cmd, exec);
	}
	free_exec(exec);
}
