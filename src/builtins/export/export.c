/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 01:51:43 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 13:32:56 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_a_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_args_only(t_exec *exec)
{
	char	**arg;
	int		i;

	arg = exec->shell->commands->args;
	i = 1;
	while (arg[i])
	{
		if (ft_strchr(arg[i], '='))
		{
			i++;
			continue ;
		}
		if (!is_a_valid_identifier(arg[i]))
		{
			print_error("export", arg[i], "not a valid identifier");
			return (1);
		}
		else if (get_env_var(arg[i], exec->shell->env) == NULL)
		{
			if (!set_env_var(arg[i], "", exec->shell->env, 0))
				return (1);
		}
		i++;
	}
	return (0);
}

int	builtin_export(t_exec *exec)
{
	int	error;

	error = 0;
	if (exec->nb_arg == 1)
		print_env_sorted(exec);
	else if (exec->shell->commands->assignments)
		error = export_with_assignment(exec);
	else if (exec->nb_arg > 1)
		error = export_args_only(exec);
	return (error);
}
