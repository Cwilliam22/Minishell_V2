/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_redir_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:02:17 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/19 18:02:17 by alfavre          ###   ########.ch       */
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
		redir->heredoc = create_heredoc(target, single_quote(target));
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

/**
 * Get string representation of redirection type
 * @param index: Index of the redirection type
 * @return: String representation
 */
const char	*get_redir_types(int index)
{
	static const char	*redir_types[] = {
		"REDIR IN (<)", "REDIR OUT (>)",
		"REDIR APPEND (>>)", "HEREDOC (<<)"
	};

	return (redir_types[index]);
}
