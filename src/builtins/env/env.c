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
		if (head->value && (ft_strcmp(head->value, "") != 0))
			printf("%s=%s\n", head->key, head->value);
		head = head->next;
	}
}

int	builtin_env(t_exec *exec)
{
	char	**arg;

	arg = exec->shell->commands->args;
	if (exec->nb_arg == 1)
		print_env(exec->shell->env);
	else if (exec->nb_arg != 1)
	{
		print_error("env", arg[1], "No such file or directory");
		return (127);
	}
	return (0);
}
