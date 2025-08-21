/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:46:00 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/14 15:46:31 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env(t_env *env)
{
	t_env_var	*head;

	head = env->variables;
	if (!env || !head)
	{
		printf("No environment variables set.\n");
		return ;
	}
	while (head)
	{
		if (head->value)
		{
			write(STDOUT_FILENO, head->key, ft_strlen(head->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, head->value, ft_strlen(head->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		head = head->next;
	}
}

int	builtin_env(t_exec *exec)
{
	char	**arg;
	char	*pwd;
	char	*new_command;

	arg = exec->shell->commands->args_expanded;
	pwd = getcwd(NULL, 0);
	new_command = ft_strjoin(pwd, "/env");
	free(pwd);
	set_env_var("_", new_command, exec->shell->env, 0);
	free(new_command);
	if (arg[1])
	{
		print_error("env", arg[1], "Too many arguments");
		return (127);
	}
	print_env(exec->shell->env);
	return (0);
}
