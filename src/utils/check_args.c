/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:58:30 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:58:30 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_all_spaces(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/**
* @brief Validates command arguments in execution context
* @param exec Execution context containing arguments to validate
* @return 1 if arguments are valid, 0 otherwise
*/
int	check_args(t_exec *exec)
{
	t_shell	*shell;
	char	**args;
	char	*cmd_name;

	shell = exec->shell;
	if (!shell || !shell->commands)
		return (0);
	args = shell->commands->args_expanded;
	if (!args || !args[0])
		return (0);
	cmd_name = args[0];
	if (!cmd_name || cmd_name[0] == '\0')
	{
		print_error("Command", "''", "not found");
		set_exit_status(127);
		return (0);
	}
	if (is_all_spaces(cmd_name))
	{
		print_error(NULL, cmd_name, "command not found");
		set_exit_status(127);
		return (0);
	}
	return (1);
}
