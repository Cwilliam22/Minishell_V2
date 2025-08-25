/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_permission.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:57:02 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:57:37 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_directory_error(char *name_cmd, int from_path_search)
{
	if (from_path_search)
	{
		print_error(name_cmd, NULL, "command not found");
		set_exit_status(127);
	}
	else
	{
		print_error(name_cmd, NULL, "Is a directory");
		set_exit_status(126);
	}
}

static void	handle_permission_error(char *name_cmd, int from_path_search)
{
	if (from_path_search)
	{
		print_error(name_cmd, NULL, "command not found");
		set_exit_status(127);
	}
	else
	{
		print_error(name_cmd, NULL, "Permission denied");
		set_exit_status(126);
	}
}

/**
* @brief Checks command permissions and handles error messages
* @param name_cmd Command path to check
* @param from_path_search 1 if found via PATH search, 0 if direct path
* @return 1 if executable, 0 otherwise
*/
int	command_permission(char *name_cmd, int from_path_search)
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
		handle_directory_error(name_cmd, from_path_search);
		return (0);
	}
	if (access(name_cmd, X_OK) != 0)
	{
		handle_permission_error(name_cmd, from_path_search);
		return (0);
	}
	return (1);
}
