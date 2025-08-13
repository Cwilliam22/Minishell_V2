/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:33:41 by alexis            #+#    #+#             */
/*   Updated: 2025/08/13 14:09:29 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Expand variables in assignments
 * @param assignments: List of assignments to expand
 * @param shell: Shell context
 */
void	expand_assignments(t_ass *assignments, t_shell *shell)
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
 * Expand variables in heredoc delimiter
 * @param heredoc: Heredoc to expand
 * @param shell: Shell context
 */
static void	expand_heredoc(t_heredoc *heredoc, t_shell *shell)
{
	char	*expanded_delimiter;

	if (!heredoc || !heredoc->delimiter)
		return ;
	heredoc->quoted_delimiter = has_quotes(heredoc->delimiter);
	expanded_delimiter = handle_quotes(heredoc->delimiter, shell);
	free(heredoc->delimiter);
	heredoc->delimiter = expanded_delimiter;
}

/**
 * Expand variables in redirections
 * @param redirections: List of redirections to expand
 * @param shell: Shell context
 */
void	expand_redirections(t_redir *redirections, t_shell *shell)
{
	t_redir	*current;
	char	*expanded_file;

	current = redirections;
	while (current)
	{
		if (current->type == REDIR_HEREDOC && current->heredoc)
			expand_heredoc(current->heredoc, shell);
		else if (current->file)
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
void expand_command_args(t_cmd *cmd, t_shell *shell)
{
	int i, original_count, valid_count;
	char **old_args, **new_args;
	char *expanded;

	if (!cmd || !cmd->args)
		return ;
	old_args = cmd->args;
	original_count = 0;
	while (old_args[original_count])
		original_count++;
	new_args = safe_malloc(sizeof(char*) * (original_count + 1));
	valid_count = 0;
	for (i = 0; i < original_count; i++)
	{
		if (!ft_strchr(old_args[i], '='))
		{
			expanded = handle_quotes(old_args[i], shell);
			if (ft_strcmp(expanded, "") != 0)
			{
				new_args[valid_count] = expanded;
				valid_count++;
			}
			else
			{
				free(expanded);
			}
		}
		else
		{
			new_args[valid_count++] = ft_strdup(old_args[i]);
		}
	}
	new_args[valid_count] = NULL;
	cmd->args_expanded = new_args;
}
