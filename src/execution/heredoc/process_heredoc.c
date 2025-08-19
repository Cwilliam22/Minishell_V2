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

int	process_hd(t_redir *redir)
{
	pid_t	pid;

	if (!redir || !redir->heredoc)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		read_and_write_heredoc(redir);
		//close_and_exit();
	}
	return (1);
}
