/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 04:45:06 by alexis            #+#    #+#             */
/*   Updated: 2025/08/04 18:55:17 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_direct(t_exec *exec)
{
	if (is_builtin(exec->shell->commands))
		return (execute_builtin(exec));
	else
		return (execute_externe(exec));
}

static int	execute_with_redirections(t_exec *exec)
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
	redir_status = apply_redirections(exec);
	if (redir_status != 0)
	{
		restore_std(saved_stdout, saved_stdin);
		return (redir_status);
	}
	exit_code = execute_direct(exec);
	restore_std(saved_stdout, saved_stdin);
	return (exit_code);
}

//Anciennement identification
void	execute_single_command(t_exec *exec)
{
	int		exit_code;

	exit_code = 0;
	exec->current_cmd = exec->shell->commands;
	exec->nb_arg = get_nb_command_args(exec->current_cmd);
	if (has_redirections(exec->current_cmd))
		exit_code = execute_with_redirections(exec);
	else
		exit_code = execute_direct(exec);
	set_exit_status(exit_code);
}
