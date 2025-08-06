/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:36:34 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 17:12:40 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_exit_status(int exit_value)
{
	t_shell	*shell;

	shell = get_shell(NULL);
	if (!shell || !shell->env)
		return ;
	shell->env->last_exit_status = exit_value;
}

/**
 * Print welcome message
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

t_shell	*get_shell(char **envp)
{
	static t_shell	*shell = NULL;

	if (!shell)
		shell = init_shell(envp);
	return (shell);
}
