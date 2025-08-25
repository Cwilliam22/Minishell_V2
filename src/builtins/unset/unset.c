/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:54:41 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:54:41 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the built-in 'unset' command.
 * 
 * @param exec Execution context containing unset arguments
 * @return int 0 on success, 1 on failure
 */
int	builtin_unset(t_exec *exec)
{
	int		i;
	char	**args;
	int		exit_code;

	i = 1;
	exit_code = 0;
	args = exec->shell->commands->args_expanded;
	if (!args[1])
		return (exit_code);
	while (args[i])
	{
		remove_env_var(args[i], exec->shell->env);
		i++;
	}
	return (exit_code);
}
