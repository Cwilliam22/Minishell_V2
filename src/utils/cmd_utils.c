/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:55:32 by alexis            #+#    #+#             */
/*   Updated: 2025/08/05 12:41:52 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	its_absolute_path(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (cmd->args[0][0] == '/')
		cmd->cmd_path = ft_strdup(cmd->args[0]);
	if (!cmd->cmd_path)
		return (0);
	return (1);
}

static int	its_relative_path(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '/')
		cmd->cmd_path = ft_strdup(cmd->args[0]);
	if (!cmd->cmd_path)
		return (0);
	return (1);
}

int	update_state_path(t_exec *exec)
{
	t_shell	*shell;
	t_cmd	*current;

	shell = exec->shell;
	if (!shell || !shell->commands)
		return (0);
	current = shell->commands;
	while (current)
	{
		if (its_absolute_path(current))
			return (current->state_path = PATH_ABSOLUTE, 1);
		if (its_relative_path(current))
			return (current->state_path = PATH_RELATIVE, 1);
		current->state_path = PATH_SIMPLE;
		current = current->next;
	}
	return (1);
}
// jamais appelee?
/*void	update_cmd_path(t_exec *exec)
{
	if (exec->shell->commands->args[0][0] == '/')
		exec->current_cmd->cmd_path = ft_strdup(exec->shell->commands->args[0]);
	else if (exec->shell->commands->args[0][0] == '.'
		&& exec->shell->commands->args[0][1] == '/')
		exec->current_cmd->cmd_path = ft_strdup(exec->shell->commands->args[0]);
	if (!exec->current_cmd->cmd_path)
	{
		print_error(NULL, NULL, "malloc failed");
		cleanup_and_exit(2);
	}
}*/
