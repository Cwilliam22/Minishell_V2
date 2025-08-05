/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/30 16:53:22 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Initialize shell structure with default values
 * @param envp: System environment variables
 * @return: Shell structure or NULL on error
 */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = get_shell(envp);
	shell->env = init_environment(envp);
	shell->commands = NULL;
	shell->input_line = NULL;
	shell->interactive = isatty(STDIN_FILENO);
	shell->running = 1;
	return (shell);
}

