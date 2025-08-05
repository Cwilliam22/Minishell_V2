/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:30:25 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 00:17:41 by alexis           ###   ########.fr       */
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

int	check_args(t_exec *exec)
{
	t_shell	*shell;
	char	**process;

	shell = exec->shell;
	if (!shell || !shell->commands || !shell->commands->args)
		return (GENERAL_ERROR);
	process = shell->commands->args;
	if (!process || !process[0] || !process[0][0] || is_all_spaces(process[0]))
	{
		if (process[0] && process[0][0] == '\0')
			print_error("Command", process[0], "not found");
		else if (is_all_spaces(process[0]))
			print_error(NULL, process[0], "command not found");
		set_exit_status(127);
		return (0);
	}
	return (1);
}
