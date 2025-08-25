/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:59:51 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:00:34 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Finds command executable in filesystem paths
* @param cmd Command structure to find path for
* @return 1 if found, 0 otherwise
*/
int	find_other_in_path(t_cmd *cmd)
{
	char	*name_cmd;

	name_cmd = cmd->args_expanded[0];
	if (check_command_exist(name_cmd))
	{
		if (cmd->cmd_path)
			free(cmd->cmd_path);
		cmd->cmd_path = ft_strdup(name_cmd);
		return (1);
	}
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
	return (0);
}

static int	search_in_current_dir(t_cmd *cmd)
{
	char	*full_path;

	full_path = ft_strjoin("./", cmd->args_expanded[0]);
	if (!full_path)
		return (0);
	if (access(full_path, F_OK) == 0)
	{
		free(cmd->cmd_path);
		cmd->cmd_path = full_path;
		return (1);
	}
	free(full_path);
	return (0);
}

/**
* @brief Searches for command executable in PATH environment variable
* @param cmd Command structure to search for
* @param exec Execution context containing environment
* @return 1 if found, 0 otherwise
*/
int	search_in_path(t_cmd *cmd, t_exec *exec)
{
	char	**paths;
	int		result;

	result = 0;
	if (search_in_current_dir(cmd))
		return (1);
	if (!exec->path || !exec->path[0])
		return (0);
	paths = ft_split(exec->path, ':');
	if (!paths || !paths[0])
	{
		free_array(paths);
		return (0);
	}
	if (find_simple_in_path(cmd, paths))
		result = 1;
	free_array(paths);
	return (result);
}
