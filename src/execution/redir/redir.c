/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:36:58 by root              #+#    #+#             */
/*   Updated: 2025/08/03 05:47:29 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Apply all redirections in the list
 * @param redirs: List of redirections to apply
 * @param shell: Shell structure for signal handling
 * @return: 0 on success, 1 on error, EXIT_SIGINT if interrupted
 */
int	apply_redirections(t_cmd *cmd)
{
	t_redir	*curr;
	int		ret;

	curr = cmd->redirections;
	while (curr)
	{
		if (g_signal_received == SIGINT)
			return (EXIT_SIGINT);
		if (curr->type == REDIR_IN)
			ret = open_and_dup(curr->file, O_RDONLY, STDIN_FILENO);
		else if (curr->type == REDIR_OUT)
			ret = open_and_dup(curr->file, O_WRONLY | O_CREAT
					| O_TRUNC, STDOUT_FILENO);
		else if (curr->type == REDIR_APPEND)
			ret = open_and_dup(curr->file, O_WRONLY | O_CREAT
					| O_APPEND, STDOUT_FILENO);
		else if (curr->type == REDIR_HEREDOC)
			ret = handle_heredoc(curr->heredoc->delimiter);
		else
			ret = 0;
		if (ret)
			return (ret);
		curr = curr->next;
	}
	return (0);
}

int	has_redirections(t_cmd *commands)
{
	t_cmd	*head;

	head = commands;
	while (head)
	{
		if (head->redirections)
			return (1);
		head = head->next;
	}
	return (0);
}
