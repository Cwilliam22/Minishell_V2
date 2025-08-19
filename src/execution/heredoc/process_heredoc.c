#include "minishell.h"

static void	expand_heredoc_content(t_redir *redir, char *line)
{
	char	*new_line;
	t_shell	*shell;

	shell = get_shell(NULL);
	if (redir->heredoc->quoted_delimiter)
	{
		new_line = ft_strdup(line);
		if (!new_line)
			return ;
	}
	else
		new_line = expand_variables(line, shell);
	ft_putstr_fd(new_line, redir->heredoc->fd);
	ft_putstr_fd("\n", redir->heredoc->fd);
}

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

void	process_hd(t_redir *redir)
{
	t_heredoc	*hd;
	pid_t		pid;
	t_shell		*shell;

	shell = get_shell(NULL);
	if (!redir || !redir->heredoc)
		return ;
	hd = redir->heredoc;
	heredoc_parent_signal();
	pid = fork();
	if (pid < 0)
	{
		error_fork(hd);
		return ;
	}
	if (pid == 0)
	{
		heredoc_child_signal();
		read_and_write_heredoc(redir);
		exit(shell->env->last_exit_status);
	}
	wait_child(pid);
	if (hd->fd >= 0)
		close(hd->fd);
	if (hd->path)
		unlink(hd->path);
	parent_signal();
}
