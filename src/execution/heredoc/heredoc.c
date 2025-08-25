/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:25:31 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:25:35 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if commands contain heredoc redirections
 * @param cmds Command list to check
 * @return 1 if heredocs found, 0 otherwise
 */
int	has_heredocs(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmds;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (current_redir->type == REDIR_HEREDOC)
				return (1);
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

/**
 * @brief Handles all heredoc redirections in commands
 * @param cmds Command list containing heredocs
 * @return 0 on success, -1 on error
 */
int	handle_heredocs(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmds;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (current_redir->type == REDIR_HEREDOC)
			{
				if (create_heredoc_file(current_redir) == 1)
					return (1);
				if (process_heredoc(current_redir) == 1)
					return (1);
				free(current_redir->file);
				current_redir->file = ft_strdup(current_redir->heredoc->path);
				current_redir->type = REDIR_IN;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}
