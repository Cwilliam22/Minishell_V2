/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:46:10 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 15:12:59 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Cleanup current input iteration (commands and input line)
 * @param shell: Shell structure
 */
void	cleanup_iteration(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->input_line)
	{
		free(shell->input_line);
		shell->input_line = NULL;
	}
}

void	cleanup_all(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->shell)
	{
		cleanup_shell(exec->shell);
		exec->shell = NULL;
	}
	free_exec(exec);
}

/**
 * Complete cleanup of shell structure
 * @param shell: Shell structure to cleanup
 */
void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	cleanup_iteration(shell);
	if (shell->env)
		free_environment(shell->env);
	if (shell->interactive)
		clear_history();
	free(shell);
}

void	cleanup_and_exit(int exit_code)
{
	t_shell	*shell;

	shell = get_shell(NULL);
	if (shell)
		cleanup_shell(shell);
	exit(exit_code);
}
