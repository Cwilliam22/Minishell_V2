/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:05:21 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/06 15:11:12 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_shell *shell)
{
	t_exec	*exec;

	exec = create_exec(shell);
	if (!exec)
		return ;
	update_var_path(exec);
	if (shell->commands && shell->commands->args_expanded[0])
	{
		exec->current_cmd = shell->commands;
		if (exec->nb_process == 1)
		{
			if (check_args(exec))
				execute_single_command(exec);
		}
		else if (exec->nb_process > 1)
			handle_pipeline(exec);
	}
	free_exec(exec);
}
