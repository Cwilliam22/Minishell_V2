/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:57:54 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:57:54 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_non_simple_path(t_cmd *cmd)
{
	if (!find_other_in_path(cmd))
	{
		print_error(cmd->args_expanded[0], NULL,
			"No such file or directory");
		set_exit_status(127);
		return (0);
	}
	if (!command_permission(cmd->cmd_path, 0))
		return (0);
	return (1);
}

static int	handle_simple_path(t_cmd *cmd, t_exec *exec)
{
	int	search;

	search = search_in_path(cmd, exec);
	if (search == 0)
	{
		print_error(cmd->args_expanded[0], NULL, "command not found");
		set_exit_status(127);
		return (0);
	}
	return (command_permission(cmd->cmd_path, 1));
}

/**
* @brief Applies command path resolution and validation
* @param cmd Command structure to resolve path for
* @param exec Execution context structure
* @return 1 on success, 0 on error
*/
int	apply_cmd_path(t_cmd *cmd, t_exec *exec)
{
	if (!cmd)
		return (0);
	if (cmd->state_path != PATH_SIMPLE)
		return (handle_non_simple_path(cmd));
	return (handle_simple_path(cmd, exec));
}
