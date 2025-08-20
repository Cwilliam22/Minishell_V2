/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcapt < wcapt@student.42lausanne.ch >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:35:22 by wcapt             #+#    #+#             */
/*   Updated: 2025/08/20 12:02:35 by wcapt            ###   ########.fr       */
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
			close(redir->heredoc->fd);
			// clean_up_all(exec)
			exit(EXIT_FAILURE);
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
	pid_t	pid;

	exit_status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
			else
				exit_status = EXIT_FAILURE;
			set_exit_status(exit_status);
		}
	}
}

static void	in_child(t_redir *redir, int exit_status)
{
	t_shell		*shell;
	t_exec		*exec;

	shell = get_shell(NULL);
	heredoc_child_signal();
	read_and_write_heredoc(redir);
	exec = get_exec();
	exit_status = shell->env->last_exit_status;
	cleanup_all(exec);
	free(exec);
	exit(exit_status);
}

void	process_hd(t_redir *redir)
{
	t_heredoc	*hd;
	pid_t		pid;
	int			exit_status;

	if (!redir || !redir->heredoc)
		return ;
	hd = redir->heredoc;
	exit_status = 0;
	heredoc_parent_signal();
	pid = fork();
	if (pid < 0)
	{
		error_fork(hd);
		return ;
	}
	if (pid == 0)
		in_child(redir, exit_status);
	wait_child(pid);
	if (hd->fd >= 0)
	{
		close(hd->fd);
		hd->fd = -1;
	}
	parent_signal();
}
