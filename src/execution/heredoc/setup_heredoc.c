/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcapt < wcapt@student.42lausanne.ch >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:24:36 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 12:07:02 by wcapt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
