/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:20:38 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/04 19:00:21 by alfavre          ###   ########.fr       */
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

	value = get_env_var("SHLVL", shell->env);
	if (!value)
	{
		if (!set_env_var("SHLVL", "0",shell->env, 0))
			return (printf("failed to set variable\n"), GENERAL_ERROR);
	}
	shlvl = ft_atoi(value);
	shlvl++;
	free(value);
	new_lvl = ft_itoa(shlvl);
	if (!new_lvl)
		return (GENERAL_ERROR); //Exit ??
	if (!set_env_var("SHLVL", new_lvl, shell->env, 0))
		return (free(new_lvl), GENERAL_ERROR);
	return (free(new_lvl), SUCCESS);
}
