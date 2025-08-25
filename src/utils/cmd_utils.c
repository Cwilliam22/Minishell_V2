/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:01:23 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:01:27 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Updates the path state of a command
* @param cmd Command structure to update path state for
* @return 1 on success, 0 on error
*/
int	update_state_path(t_cmd *cmd)
{
	char	*first_arg;

	if (!cmd || !cmd->args_expanded || !cmd->args_expanded[0])
		return (0);
	first_arg = cmd->args_expanded[0];
	if (first_arg[0] == '/')
	{
		cmd->state_path = PATH_ABSOLUTE;
		return (1);
	}
	if (ft_strchr(first_arg, '/'))
	{
		cmd->state_path = PATH_RELATIVE;
		return (1);
	}
	cmd->state_path = PATH_SIMPLE;
	return (1);
}

/**
* @brief Checks if a command exists in the system
* @param name Command name to check
* @return 1 if command exists, 0 otherwise
*/
int	check_command_exist(char *name)
{
	if (access(name, F_OK) != 0)
	{
		set_exit_status(127);
		return (0);
	}
	return (1);
}
