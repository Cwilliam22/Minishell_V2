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

static int	count_valid_args(char **args, int start)
{
	int	count;

	count = 0;
	while (args[start + count])
		count++;
	return (count);
}

static char	**create_new_args(char **args, int start, int size)
{
	char	**new_args;
	int		j;

	new_args = malloc(sizeof(char *) * (size + 1));
	if (!new_args)
		return (NULL);
	j = 0;
	while (args[start])
	{
		new_args[j] = ft_strdup(args[start]);
		if (!new_args[j])
		{
			free_array(new_args);
			return (NULL);
		}
		j++;
		start++;
	}
	new_args[j] = NULL;
	return (new_args);
}

void	first_valid_argument(t_cmd *cmd)
{
	int		i;
	int		size;
	char	**new_args;

	if (!cmd || !cmd->args_expanded)
		return ;
	i = 0;
	while (cmd->args_expanded[i] && cmd->args_expanded[i][0] == '\0')
		i++;
	if (!cmd->args_expanded[i])
	{
		free_array(cmd->args_expanded);
		cmd->args_expanded = NULL;
		return ;
	}
	size = count_valid_args(cmd->args_expanded, i);
	new_args = create_new_args(cmd->args_expanded, i, size);
	if (!new_args)
		return ;
	free_array(cmd->args_expanded);
	cmd->args_expanded = new_args;
}
