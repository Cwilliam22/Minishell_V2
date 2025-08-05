/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:17:28 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 00:27:58 by alexis           ###   ########.fr       */
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
		expand_command_args(current, shell);
		if (current->assignments)
			expand_assignments(current->assignments, shell);
		if (current->redirections)
			expand_redirections(current->redirections, shell);
		current = current->next;
	}
}
