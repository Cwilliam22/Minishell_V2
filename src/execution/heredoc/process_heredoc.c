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

static int	read_and_write_heredoc(t_redir *redir)
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
	return (1);
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

void	in_child(t_redir *redir)
{
	int ok;
	struct sigaction sa = {0};
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	ok = read_and_write_heredoc(redir);
	if (ok)
		exit(EXIT_SUCCESS);
	else
		exit(EXIT_FAILURE);
}

int	child_conditions(t_heredoc *hd, int status)
{
	// Ctrl-C
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		set_exit_status(130);
		if (hd->fd >= 0)
			close(hd->fd);
		if (hd->path)
			unlink(hd->path);
		return (0);
	}
	// l'enfant -> retourne une erreur
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		set_exit_status(1);
		if (hd->fd >= 0)
			close(hd->fd);
		if (hd->path)
			unlink(hd->path);
		return (0);
	}
	else
		return (1);
}

int	process_hd(t_redir *redir)
{
	t_heredoc	*hd;
	pid_t		pid;
	int			status;

	if (!redir || !redir->heredoc)
		return (0);
	hd = redir->heredoc;
	pid = fork();
	if (pid < 0)
		return (error_fork(hd));
	if (pid == 0) 
		in_child(redir);
	// parent -> attendre l'enfant
	if (waitpid(pid, &status, 0) < 0)
	{
		print_error(NULL, NULL, "waitpid failed");
		set_exit_status(1);
		if (hd->fd >= 0)
			close(hd->fd);
		if (hd->path)
			unlink(hd->path);
		return (0);
	}
	if (!child_conditions(hd, status))
		return (0);
	return (1);
}
