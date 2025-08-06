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
	t_cmd	*current;

	exec = create_exec(shell);
	if (!exec)
		return ;
	update_var_path(exec);
	current = shell->commands;
	while (current)
	{
		if (!current->args || !current->args[0])
		{
			current = current->next;
			continue ;
		}
		exec->current_cmd = current;
		if (check_args(exec))
			execute_single_command(exec);
		current = current->next;
	}
	free_exec(exec);
}
