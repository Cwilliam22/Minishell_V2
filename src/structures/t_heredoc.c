/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:57:48 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/19 17:58:08 by alfavre          ###   ########.ch       */
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
	heredoc->path = NULL;
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
	if (heredoc->path)
		free(heredoc->path);
	free(heredoc);
}

