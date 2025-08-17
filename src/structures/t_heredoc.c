/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:31:36 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 14:35:48 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Create a new heredoc structure
 * @param delimiter: Delimiter string for the heredoc
 * @param quoted: Whether the delimiter was quoted
 * @return: Pointer to the newly created t_heredoc structure, or NULL on failure
 */
t_heredoc	*create_heredoc(char *delimiter, int quoted)
{
	t_heredoc	*heredoc;

	if (!delimiter)
		return (NULL);
	heredoc = (t_heredoc *)safe_malloc(sizeof(t_heredoc));
	heredoc->delimiter = ft_strdup(delimiter);
	if (!heredoc->delimiter)
	{
		free(heredoc);
		return (NULL);
	}
	heredoc->content = NULL;
	heredoc->quoted_delimiter = quoted;
	heredoc->fd = -1;
	heredoc->id = 0;
	return (heredoc);
}

/**
 * Free a single heredoc
 * @param heredoc: Heredoc to free
 */
void	free_heredoc(t_heredoc *heredoc)
{
	if (!heredoc)
		return ;
	if (heredoc->delimiter)
		free(heredoc->delimiter);
	if (heredoc->content)
		free(heredoc->content);
	if (heredoc->fd != -1)
		close(heredoc->fd);
	free(heredoc);
}

