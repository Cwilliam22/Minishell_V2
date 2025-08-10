/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:17:28 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 15:28:17 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Expand variables in all commands
 * @param cmd: Command to expand
 * @param shell: Shell context
 */
void	expand_commands(t_shell *shell)
{
	t_cmd	*current;

	current = shell->commands;
	if (!current || !shell)
		return ;
	while (current)
	{
		if (current->assignments)
			expand_assignments(current->assignments, shell);
		if (current->redirections)
			expand_redirections(current->redirections, shell);
		expand_command_args(current, shell);
		current = current->next;
	}
}
