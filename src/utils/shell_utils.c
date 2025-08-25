/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:05:02 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:06:06 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Gets the current exit status from execution context
* @param exec Execution context containing exit status
* @return Current exit status value
*/
int	get_exit_status(t_exec *exec)
{
	if (!exec || !exec->shell || !exec->shell->env)
		return (0);
	return (exec->shell->env->last_exit_status);
}

/**
* @brief Sets the global exit status value
* @param exit_value Exit status value to set
*/
void	set_exit_status(int exit_value)
{
	t_shell	*shell;

	shell = get_shell(NULL);
	if (!shell || !shell->env)
		return ;
	shell->env->last_exit_status = exit_value;
}

/**
* @brief Prints welcome message for the shell
*/
void	print_welcome(void)
{
	printf("\n");
	printf("╔══════════════════════════════════════╗\n");
	printf("║           🐚 MINISHELL 🐚            ║\n");
	printf("║                                      ║\n");
	printf("║      A beautiful shell experience    ║\n");
	printf("║                                      ║\n");
	printf("║  Type 'exit' to quit                 ║\n");
	printf("║  Use ctrl-C, ctrl-D as in bash       ║\n");
	printf("╚══════════════════════════════════════╝\n");
	printf("\n");
}

/**
* @brief Creates and initializes shell structure with environment
* @param envp Environment variables array
* @return Pointer to initialized shell structure
*/
t_shell	*get_shell(char **envp)
{
	static t_shell	*shell = NULL;

	if (!shell)
		shell = init_shell(envp);
	return (shell);
}
