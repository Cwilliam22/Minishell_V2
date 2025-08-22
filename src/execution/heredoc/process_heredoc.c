/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:27:17 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 22:49:54 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_and_write_heredoc(t_redir *redir)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_error(NULL, "warning", "here-document delimited by eof");
			break ;
		}
		if (ft_strcmp(line, redir->heredoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		expand_heredoc_content(redir, line);
		free(line);
	}
}

int	error_fork(t_heredoc *hd)
{
	print_error(NULL, NULL, "fork failed");
	set_exit_status(1);
	if (hd->fd >= 0)
		close(hd->fd);
	if (hd->path)
		unlink(hd->path);
	return (0);
}

void	wait_child(pid_t last_pid)
{
	int		status;
	int		exit_status;

	exit_status = 0;
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			exit_status = EXIT_SIGINT;
		else
			exit_status = 128 + WTERMSIG(status);
	}
	set_exit_status(exit_status);
}

static void	in_child(t_redir *redir)
{
	heredoc_child_signal();
	read_and_write_heredoc(redir);
	exit(0);
}

int	process_heredoc(t_redir *redir)
{
	t_heredoc	*hd;
	pid_t		pid;

	if (!redir || !redir->heredoc)
		return (0);
	hd = redir->heredoc;
	heredoc_parent_signal();
	pid = fork();
	if (pid < 0)
	{
		error_fork(hd);
		return (1);
	}
	if (pid == 0)
		in_child(redir);
	wait_child(pid);
	if (hd->fd >= 0)
	{
		close(hd->fd);
		hd->fd = -1;
	}
	parent_signal();
	return (0);
}
