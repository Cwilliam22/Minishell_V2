
#include "minishell.h"

void	number_of_hd(t_redir *redir)
{
	t_heredoc	*head;

	head = redir->heredoc;
	while (head)
	{
		redir->nb_hd++;
		head = head->next;
	}
}

int	unquote_delimiter(t_heredoc *heredoc)
{
	char *tmp;

	tmp = ft_strtrim(heredoc->delimiter, "\'");
	if (!tmp)
		return (0);
	free(heredoc->delimiter);
	heredoc->delimiter = tmp;
	return (1);
}

int	setup_for_heredoc(t_heredoc *heredoc)
{
	if (!heredoc)
		return (0);
	if (heredoc->quoted_delimiter)
	{
		if (!unquote_delimiter(heredoc->delimiter))
			cleanup_and_exit(2);
	}
	return (1);
}

void	heredoc(t_redir *redir)
{
	t_shell *shell;
	t_heredoc *hd;

	shell = get_shell(NULL);
	if (!redir || !redir->heredoc)
		return ;
	number_of_hd(redir);
	hd = redir->heredoc;
	while (hd)
	{
		setup_for_heredoc(hd);
		hd = hd->next;
	}

}
