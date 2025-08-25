/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:35:31 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:35:31 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_permission(char *name_cmd)
{
	struct stat	file_stat;

	if (stat(name_cmd, &file_stat) != 0)
	{
		print_error(name_cmd, NULL, "No such file or directory");
		set_exit_status(127);
		return (0);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		print_error(name_cmd, NULL, "Is a directory");
		set_exit_status(126);
		return (0);
	}
	if (access(name_cmd, X_OK) != 0)
	{
		print_error(name_cmd, NULL, "Permission denied");
		set_exit_status(126);
		return (0);
	}
	return (1);
}

/**
 * Fonction qui contrôle si une commande est executable ou si elle existe
 * en fonction de l'appelant
 * @param shell: structure du shell
 * @param access_mode: F_OK (existence) ou X_OK (exécutable)
 * @return: 1 si trouvé, 0 sinon
 */
int	apply_cmd_path(t_cmd *cmd, t_exec *exec)
{
	if (!cmd)
		return (0);
	if (cmd->state_path != PATH_SIMPLE)
	{
		if (!find_other_in_path(cmd))
		{
			print_error(cmd->args_expanded[0], NULL,
				"No such file or directory");
			set_exit_status(127);
			return (0);
		}
		if (!command_permission(cmd->cmd_path))
			return (0);
		return (1);
	}
	else
	{
		if (search_in_path(cmd, exec))
			return (1);
		print_error(cmd->args_expanded[0], NULL, "command not found");
		set_exit_status(127);
		return (0);
	}
}
