/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:24:36 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 20:05:57 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_iter_char(const char *str, int c)
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

static int	type_of_quote(const char *str)
{
	int	single_quote;
	int	double_quote;

	if (!str)
		return (0);
	single_quote = ft_iter_char(str, '\'');
	double_quote = ft_iter_char(str, '\"');
	return (conditions_type_of_quotes(single_quote, double_quote, str));
}

static int	unquote_delimiter(t_heredoc *heredoc)
{
	char	*tmp;

	if (heredoc->quoted_delimiter == DOUBLE_QUOTED)
	{
		tmp = ft_strtrim(heredoc->delimiter, "\"");
		if (!tmp)
			return (0);
		free(heredoc->delimiter);
		heredoc->delimiter = tmp;
	}
	if (heredoc->quoted_delimiter == SIMPLE_QUOTED)
	{
		tmp = ft_strtrim(heredoc->delimiter, "\'");
		if (!tmp)
			return (0);
		free(heredoc->delimiter);
		heredoc->delimiter = tmp;
	}
	return (1);
}

static int	update_delimiter(t_heredoc *heredoc)
{
	if (!heredoc || !heredoc->delimiter)
		return (0);
	heredoc->quoted_delimiter = type_of_quote(heredoc->delimiter);
	if (heredoc->quoted_delimiter != HALF_QUOTED)
	{
		if (heredoc->quoted_delimiter != NO_QUOTED)
		{
			if (!unquote_delimiter(heredoc))
				return (0);
		}
	}
	else
	{
		print_error(NULL, NULL, "heredoc delimiter not fully quoted");
		return (set_exit_status(2), 0);
	}
	return (1);
}

int	create_heredoc_file(t_redir *redir)
{
	char	*filename;
	char	*id;

	if (!redir || !redir->heredoc)
		return (0);
	if (!update_delimiter(redir->heredoc))
		return (1);
	id = ft_itoa(redir->heredoc->id);
	if (!id)
		return (1);
	filename = ft_strjoin("/tmp/heredoc_", id);
	if (!filename)
		return (free(id), 1);
	free(id);
	redir->heredoc->path = filename;
	return (0);
}
