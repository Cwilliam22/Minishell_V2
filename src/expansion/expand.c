/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:55:15 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 13:55:25 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Expand variables in assignments
 * @param assignments: List of assignments to expand
 * @param shell: Shell context
 */
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

/**
 * Expand variables in redirections
 * @param redirections: List of redirections to expand
 * @param shell: Shell context
 */
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
 * Version complète de expand_command_args avec handle_quotes
 * @param cmd: Command to expand
 * @param shell: Shell context
 */
/*static void	expand_command_args(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		j;
	char	*expanded;
	int		size;

	i = 0;
	j = 0;
	size = get_nb_command_args(cmd);
	cmd->args_expanded = (char **)safe_malloc(sizeof(char *) * (size + 1));
	while (cmd->args[i])
	{
		expanded = handle_quotes(cmd->args[i], shell);
		if (expanded)
			cmd->args_expanded[j++] = expanded;
		i++;
	}
	cmd->args_expanded[j] = NULL;
}*/

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
