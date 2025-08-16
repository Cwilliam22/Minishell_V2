/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:49:45 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/16 11:49:58 by alfavre          ###   ########.ch       */
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
		if(current->args)
			expand_command_args(current, shell);
		current = current->next;
	}
}
