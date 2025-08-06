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

static int	write_heredoc_to_pipe(char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		printf("delimiter: %s\n", delimiter);
		printf("line : %s\n", line);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(write_fd, line, ft_strlen(line)) == -1
			|| write(write_fd, "\n", 1) == -1)
		{
			perror("write heredoc");
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	if (write_heredoc_to_pipe(delimiter, pipe_fd[1]))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	close(pipe_fd[1]);
	if (dup_and_close(pipe_fd[0], STDIN_FILENO, "heredoc"))
		return (1);
	return (0);
}
