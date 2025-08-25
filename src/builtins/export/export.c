/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:54:01 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:54:06 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_var_path(t_exec *exec)
{
	if (exec->path)
	{
		free(exec->path);
		exec->path = NULL;
	}
	exec->path = get_env_var("PATH", exec->shell->env);
}

/**
 * @brief Validates if string is a valid variable identifier.
 * 
 * @param str String to validate
 * @return int 1 if valid identifier, 0 otherwise
 */
int	is_a_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
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

	arg = exec->current_cmd->args_expanded;
	i = 1;
	while (arg[i])
	{
		if (!is_a_valid_identifier(arg[i]))
		{
			print_error("export", arg[i], "not a valid identifier");
			return (1);
		}
		else if (get_env_var(arg[i], exec->shell->env) == NULL)
		{
			if (!set_env_var(arg[i], NULL, exec->shell->env, 0))
				return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Implements the built-in 'export' command.
 * 
 * @param exec Execution context containing export arguments
 * @return int 0 on success, 1 on failure
 */
int	builtin_export(t_exec *exec)
{
	int	error;

	error = 0;
	if (exec->current_cmd->assignments)
		error = export_with_assignment(exec);
	else if (exec->current_cmd->args_expanded[1])
		error = export_args_only(exec);
	else
		print_env_sorted(exec);
	update_var_path(exec);
	return (error);
}
