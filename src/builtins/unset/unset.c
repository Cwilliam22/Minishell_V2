/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 05:52:21 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 06:26:51 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_exec *exec)
{
	int		i;
	char	**args;
	int		exit_code;

	i = 1;
	exit_code = 0;
	args = exec->shell->commands->args;
	if (!args[1])
		return (exit_code);
	while (args[i])
	{
		remove_env_var(args[i], exec->shell->env);
		i++;
	}
	return (exit_code);
}
