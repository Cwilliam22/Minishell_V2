/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:38:33 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 14:39:00 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_direct(t_cmd *cmd, t_exec *exec)
{
	if (is_builtin(cmd))
	{
		return (execute_builtin(cmd, exec));
	}
	else
	{
		return (execute_externe(cmd, exec));
	}
}

static int	execute_with_redirections(t_cmd *cmd, t_exec *exec)
{
	int	saved_stdout;
	int	saved_stdin;
	int	exit_code;
	int	redir_status;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdout == -1 || saved_stdin == -1)
	{
		if (saved_stdout != -1)
			close(saved_stdout);
		if (saved_stdin != -1)
			close(saved_stdin);
		print_error(NULL, NULL, "dup");
		return (1);
	}
	redir_status = apply_redirections(cmd);
	if (redir_status != 0)
	{
		restore_std(saved_stdout, saved_stdin);
		return (redir_status);
	}
	exit_code = execute_direct(cmd, exec);
	restore_std(saved_stdout, saved_stdin);
	return (exit_code);
}

/**
 * Nb_arg compte le nom d'argument avant expension
 */
void	execute_single_command(t_cmd *cmd, t_exec *exec)
{
	int		exit_code;

	exit_code = 0;
	exec->nb_arg = get_nb_command_args(cmd);
	if (has_redirections(cmd))
	{
		exit_code = execute_with_redirections(cmd, exec);
	}
	else
	{
		exit_code = execute_direct(cmd, exec);
	}
	set_exit_status(exit_code);
}
