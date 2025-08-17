/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:40:16 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/17 11:43:51 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Increment the shell level variable (SHLVL) in the environment.
 * This function retrieves the current SHLVL value, increments it,
 * and updates the environment variable.
 * @param shell: The shell structure containing the environment.
 * @return: 1 on success, 0 on failure.
 */
int	ft_shlvl(t_shell *shell)
{
	int		shlvl;
	char	*new_lvl;
	char	*value;

	shlvl = 0;
	value = get_env_var("SHLVL", shell->env);
	if (!value)
	{
		if (!set_env_var("SHLVL", "0", shell->env, 0))
			return (print_error(NULL, NULL, "failed to set variable"),
				GENERAL_ERROR);
	}
	else
	{
		shlvl = ft_atoi(value);
		free(value);
	}
	shlvl++;
	new_lvl = ft_itoa(shlvl);
	if (!new_lvl)
		return (GENERAL_ERROR);
	if (!set_env_var("SHLVL", new_lvl, shell->env, 0))
		return (free(new_lvl), GENERAL_ERROR);
	return (free(new_lvl), SUCCESS);
}
