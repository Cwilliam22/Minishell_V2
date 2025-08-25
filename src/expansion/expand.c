/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:31:55 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:32:04 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_assignments(t_ass *assignments, t_shell *shell)
{
	t_ass	*current;
	char	*expanded_value;

	current = assignments;
	while (current)
	{
		expanded_value = handle_quotes(current->value, shell);
		free(current->value);
		current->value = expanded_value;
		current = current->next;
	}
}

static void	expand_redirections(t_redir *redirections, t_shell *shell)
{
	t_redir	*current;
	char	*expanded_file;

	current = redirections;
	while (current)
	{
		if (current->file)
		{
			expanded_file = handle_quotes(current->file, shell);
			free(current->file);
			current->file = expanded_file;
		}
		current = current->next;
	}
}

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
		if (current->args)
			expand_command_args(current, shell);
		current = current->next;
	}
}
