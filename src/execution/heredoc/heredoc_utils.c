#include "minishell.h"

int	ft_iter_char(const char *str, int c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

int	create_file(t_redir *redir)
{
	char	*filename;
	char	*id;

	if (!redir || !redir->heredoc)
		return (0);
	id = ft_itoa(redir->heredoc->id);
	if (!id)
		return (0);
	filename = ft_strjoin("/tmp/heredoc_", id);
	if (!filename)
		return (0);
	free(id);
	redir->heredoc->fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (redir->heredoc->fd < 0)
	{
		print_error(NULL, NULL, "failed to create heredoc file");
		return (free(filename), set_exit_status(1), 0);
	}
	redir->heredoc->path = filename;
	return (1);
}
