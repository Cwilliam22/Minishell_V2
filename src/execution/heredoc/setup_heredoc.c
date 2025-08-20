/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcapt < wcapt@student.42lausanne.ch >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:36:11 by wcapt             #+#    #+#             */
/*   Updated: 2025/08/20 13:47:38 by wcapt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	type_of_quote(const char *str)
{
	int	single_quote;
	int	double_quote;

	if (!str)
		return (0);
	single_quote = ft_iter_char(str, '\'');
	double_quote = ft_iter_char(str, '\"');
	if (double_quote == 0 && single_quote != 0)
		return (SIMPLE_QUOTED);
	else if (single_quote == 0 && double_quote != 0)
		return (DOUBLE_QUOTED);
	else
		return (NO_QUOTED);
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

int	setup_for_heredoc(t_heredoc *heredoc)
{
	if (!heredoc || !heredoc->delimiter)
		return (0);
	heredoc->quoted_delimiter = type_of_quote(heredoc->delimiter);
	if (heredoc->quoted_delimiter != NO_QUOTED)
	{
		if (!unquote_delimiter(heredoc))
			return (0);
	}
	return (1);
}
