/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:34:43 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 00:43:19 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_line(t_redir *redir, char *line)
{
	char	*processed_line;

	if (!redir->heredoc->quoted_delimiter)
	{
		processed_line = expand_heredoc_content(redir, line);
		if (!processed_line)
			return (-1);
	}
	else
	{
		processed_line = ft_strdup(line);
		if (!processed_line)
			return (-1);
	}
	if (write(redir->heredoc->fd, processed_line,
			ft_strlen(processed_line)) == -1)
		return (free(processed_line), -1);
	if (write(redir->heredoc->fd, "\n", 1) == -1)
		return (free(processed_line), -1);
	free(processed_line);
	return (0);
}

static void	read_and_write_heredoc(t_redir *redir, int *result)
{
	char	*line;

	if (!redir || !redir->heredoc)
		return ;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_error(NULL, "warning", "here-document delimited by end-of-file");
			break ;
		}
		if (ft_strcmp(line, redir->heredoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (process_heredoc_line(redir, line) == -1)
		{
			free(line);
			*result = 1;
			break ;
		}
		free(line);
	}
}

static void	cleanup_heredoc_files(t_heredoc *heredoc, int result)
{
	if (!heredoc)
		return ;
	if (heredoc->fd != -1)
	{
		if (close(heredoc->fd) == -1)
		{
			print_error(NULL, heredoc->path, "Failed to close heredoc file");
			set_exit_status(1);
		}
		heredoc->fd = -1;
	}
	if (result == 1)
	{
		if (heredoc->path)
		{
			unlink(heredoc->path);
			free(heredoc->path);
			heredoc->path = NULL;
		}
	}
}

int	process_heredoc(t_redir *redir)
{
	int	result;

	if (!redir || !redir->heredoc)
		return (-1);
	result = 0;
	handle_heredoc_signal();
	if (create_heredoc_file(redir) == 1)
		return (parent_signal(), 1);
	redir->heredoc->fd = open(redir->heredoc->path, O_WRONLY | O_CREAT
			| O_TRUNC, 0644);
	if (redir->heredoc->fd < 0)
	{
		cleanup_heredoc_files(redir->heredoc, 1);
		return (parent_signal(), 1);
	}
	read_and_write_heredoc(redir, &result);
	cleanup_heredoc_files(redir->heredoc, result);
	parent_signal();
	return (result);
}
