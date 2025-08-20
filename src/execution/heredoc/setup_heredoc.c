/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:08:13 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/20 16:13:26 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	last_and_first_quotes(const char *str)
{
	int	i;
	int	first_quote;
	int	last_quote;
	
	i = 0;
	first_quote = 0;
	last_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			first_quote = i;
		i++;
	}
	while (i > 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
			last_quote = i;
		i--;
	}
	if (first_quote == 0 && last_quote == 0)
		return (NO_QUOTED);
	if (first_quote == last_quote)
		return (-1);
	if (str[first_quote] == '\'' && str[last_quote] == '\'')
		return (SIMPLE_QUOTED);
	else if (str[first_quote] == '\"' && str[last_quote] == '\"')
		return (DOUBLE_QUOTED);
	else
		return (HALF_QUOTED);
}

static int	order_of_quotes(const char *str)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && d_quote % 2 == 0)
			s_quote++;
		else if (str[i] == '\"' && s_quote % 2 != 0)
			return (HALF_QUOTED);
		if (str[i] == '\"' && s_quote % 2 == 0)
			d_quote++;
		else if (str[i] == '\"' && d_quote % 2 != 0)
			return (HALF_QUOTED);
		i++;	
	}
	return (SIMPLE_DOUBLE_QUOTED);
}

static int	type_of_quote(const char *str)
{
	int	single_quote;
	int	double_quote;

	if (!str)
		return (0);
	single_quote = ft_iter_char(str, '\'');
	double_quote = ft_iter_char(str, '\"');
	if (last_and_first_quotes(str) == -1)
		return (HALF_QUOTED);
	else if (last_and_first_quotes(str) == SIMPLE_QUOTED)
	{
		if (single_quote % 2 != 0)
			return (HALF_QUOTED);
		else
			return (SIMPLE_QUOTED);
	}
	else if (last_and_first_quotes(str) == DOUBLE_QUOTED)
	{
		if (double_quote % 2 != 0)
			return (HALF_QUOTED);
		else
			return (DOUBLE_QUOTED);
	}
	else if (last_and_first_quotes(str) == HALF_QUOTED)
	{
		if (double_quote % 2 == 0 && single_quote % 2 == 0)
			return (order_of_quotes(str));
		else
			return (HALF_QUOTED);
	}
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
