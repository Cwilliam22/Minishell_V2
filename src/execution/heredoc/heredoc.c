/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:35:59 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 10:56:33 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
