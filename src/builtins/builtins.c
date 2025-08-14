/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:29:26 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/14 16:30:14 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	char		**args;
	const char	*builtins[] = {
		"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL
	};
	int			i;

	i = 0;
	args = cmd->args_expanded;
	if (!args)
		return (0);
	while (builtins[i])
	{
		if (ft_strcmp(args[0], builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	execute_builtin(t_cmd *cmd, t_exec *exec)
{
	int				i;
	const t_builtin	tab_link[] = {
	{"cd", builtin_cd},
	{"echo", builtin_echo},
	{"pwd", builtin_pwd},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"env", builtin_env},
	{"exit", builtin_exit},
	{NULL, NULL}
	};
	char			*name_cmd;

	i = 0;
	name_cmd = cmd->args_expanded[0];
	exec->current_cmd = cmd;
	while (tab_link[i].builtin != NULL)
	{
		if (ft_strcmp(name_cmd, tab_link[i].builtin) == 0)
			return (tab_link[i].fonction(exec));
		i++;
	}
	print_error(NULL, NULL, "command not found");
	return (COMMAND_NOT_FOUND);
}
