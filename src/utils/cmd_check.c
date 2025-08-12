/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:13:07 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 18:06:41 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_other_in_path(t_cmd *cmd)
{
	char	*name_cmd;

	name_cmd = cmd->args_expanded[0];
	if (check_command_exist(name_cmd))
	{
		if (cmd->cmd_path)
			free(cmd->cmd_path);
		cmd->cmd_path = ft_strdup(name_cmd);
		if (!cmd->cmd_path)
		{
			print_error(NULL, NULL, "Memory allocation failed!");
			cleanup_and_exit(2);
		}
		return (1);
	}
	print_error(name_cmd, NULL, "No such file or directory");
	return (0);
}

static int	find_simple_in_path(t_cmd *cmd, char **paths)
{
	int		i;
	char	*name_cmd;
	char	*path_cmd;
	char	*tmp;

	i = 0;
	name_cmd = cmd->args_expanded[0];
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path_cmd = ft_strjoin(tmp, name_cmd);
		free(tmp);
		if (check_command_exist(path_cmd))
		{
			if (cmd->cmd_path)
				free(cmd->cmd_path);
			cmd->cmd_path = path_cmd;
			return (1);
		}
		free(path_cmd);
		i++;
	}
	print_error(NULL, NULL, "command not found");
	return (0);
}

/**
 * Vérifie si une commande existe dans le PATH
 */
static int	command_permission(char *name_cmd)
{
	struct stat	file_stat;

	if (stat(name_cmd, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			print_error(name_cmd, NULL, "Is a directory");
			set_exit_status(126);
			return (0);
		}
	}
	if (access(name_cmd, X_OK) != 0)
	{
		print_error(name_cmd, NULL, "Permission denied");
		set_exit_status(126);
		return (0);
	}
	if (file_stat.st_size == 0)
		return (0);
	return (1);
}

static int	apply_path(t_cmd *cmd, char **paths)
{
	if (find_simple_in_path(cmd, paths))
		return (command_permission(cmd->cmd_path));
	else
		return (0);
}

/**
 * Fonction qui contrôle si une commande est executable ou si elle existe
 * en fonction de l'appelant
 * @param shell: structure du shell
 * @param access_mode: F_OK (existence) ou X_OK (exécutable)
 * @return: 1 si trouvé, 0 sinon
 */
int	apply_cmd_path(t_cmd *cmd, t_exec * exec)
{
	char	**paths;
	int		result;

	if (!cmd)
		return (0);
	if (cmd->state_path != PATH_SIMPLE)
	{
		if (find_other_in_path(cmd))
			return (command_permission(cmd->cmd_path));
		else
			return (0);
	}
	else
	{
		if (!exec->path)
		{
			exec->current_cmd->cmd_path = NULL;
			return (print_error(NULL, NULL, "No PATH variable found!"), 0);
		}
		paths = ft_split(exec->path, ':');
		if (!paths || !paths[0])
		{
			free_array(paths);
			cmd->cmd_path = NULL;
			return (print_error(NULL, NULL, "No PATH variable found!"), 0);
		}
		result = apply_path(cmd, paths);
		free_array(paths);
		return (result);
	}
}
