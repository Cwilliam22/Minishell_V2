/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:31:06 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/06 17:23:53 by alfavre          ###   ########.fr       */
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
			write(STDOUT_FILENO, head->key, strlen(head->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, head->value, strlen(head->value));
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
	set_env_var("_", new_command, exec->shell->env, 0);
	if (exec->nb_arg == 1)
	{
		print_env(exec->shell->env);
		return (0);
	}
	else
	{
		print_error("env", arg[1], "No such file or directory");
		return (127);
	}
}
