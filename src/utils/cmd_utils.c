/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:55:32 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 14:19:44 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_command_exist(char *name)
{
	if (access(name, F_OK) != 0)
	{
		set_exit_status(127);
		return (0);
	}
	return (1);
}
