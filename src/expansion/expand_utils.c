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
void	expand_command_args(t_cmd *cmd, t_shell *shell)
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
}
