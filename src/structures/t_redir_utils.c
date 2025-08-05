/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:14:56 by root              #+#    #+#             */
/*   Updated: 2025/07/29 19:37:30 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Create a new redirection structure
 * @param type: Type of redirection (e.g., REDIR_IN, REDIR_OUT, etc.)
 * @param target: File or delimiter associated with the redirection
 * @return: Pointer to the newly created t_redir structure, or NULL on failure
 */
t_redir	*create_redirection(int type, char *target)
{
	t_redir	*redir;

	redir = (t_redir *)safe_malloc(sizeof(t_redir));
	redir->type = type;
	redir->fd_in = -1;
	redir->fd_out = -1;
	redir->next = NULL;
	if (type == REDIR_HEREDOC)
	{
		redir->file = NULL;
		redir->heredoc = create_heredoc(target, 0);
		if (!redir->heredoc)
			return (free(redir), NULL);
	}
	else
	{
		redir->file = ft_strdup(target);
		if (!redir->file)
			return (free(redir), NULL);
		redir->heredoc = NULL;
	}
	return (redir);
}

/**
 * Add a new redirection to the end of the linked list of redirections
 * @param head: Pointer to the head of the linked list
 * @param new_redir: New redirection to add
 */
void	add_redirection(t_redir **head, t_redir *new_redir)
{
	t_redir	*current;

	if (!head || !new_redir)
		return ;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/**
 * Free the memory allocated for a linked list of redirections
 * @param redirections: Pointer to the head of the linked list
 */
void	free_redirections(t_redir *redirections)
{
	t_redir	*current;
	t_redir	*next;

	current = redirections;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		if (current->heredoc)
			free_heredoc(current->heredoc);
		free(current);
		current = next;
	}
}
