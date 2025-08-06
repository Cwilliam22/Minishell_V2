/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/08/06 17:26:36 by alfavre          ###   ########.fr       */
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

	shell = (t_shell *)safe_malloc(sizeof(t_shell));
	shell->env = init_environment(envp);
	shell->commands = NULL;
	shell->input_line = NULL;
	shell->interactive = isatty(STDIN_FILENO);
	shell->running = 1;
	return (shell);
}

void	init_minimal_env_vars(t_env *env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		add_env_var(&env->variables, create_env_var("PWD", pwd));
		free(pwd);
	}
	add_env_var(&env->variables, create_env_var("OLDPWD", NULL));
}
