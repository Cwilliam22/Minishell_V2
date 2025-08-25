/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:25:52 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:25:52 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	postion_of_quotes_check(int f_quote, int l_quote, const char *str)
{
	if (f_quote == -1 && l_quote == -1)
		return (NO_QUOTED);
	if (f_quote == l_quote)
		return (-1);
	if (str[f_quote] == '\'' && str[l_quote] == '\'')
		return (SIMPLE_QUOTED);
	else if (str[f_quote] == '\"' && str[l_quote] == '\"')
		return (DOUBLE_QUOTED);
	else
		return (HALF_QUOTED);
}

static int	last_and_first_quotes(const char *str)
{
	int	i;
	int	first_quote;
	int	last_quote;

	i = 0;
	first_quote = -1;
	last_quote = -1;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && first_quote == -1)
			first_quote = i;
		i++;
	}
	while (i > 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
			last_quote = i;
		i--;
	}
	return (postion_of_quotes_check(first_quote, last_quote, str));
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

/**
 * @brief Determines quote conditions for string parsing
 * @param s_quote Single quote state
 * @param d_quote Double quote state
 * @param str String being parsed
 * @return Quote condition type
 */
int	conditions_type_of_quotes(int s_quote, int d_quote, const char *str)
{
	if (last_and_first_quotes(str) == -1)
		return (HALF_QUOTED);
	else if (last_and_first_quotes(str) == SIMPLE_QUOTED)
	{
		if (s_quote % 2 != 0)
			return (HALF_QUOTED);
		else
			return (SIMPLE_QUOTED);
	}
	else if (last_and_first_quotes(str) == DOUBLE_QUOTED)
	{
		if (d_quote % 2 != 0)
			return (HALF_QUOTED);
		else
			return (DOUBLE_QUOTED);
	}
	else if (last_and_first_quotes(str) == HALF_QUOTED)
	{
		if (d_quote % 2 == 0 && s_quote % 2 == 0)
			return (order_of_quotes(str));
		else
			return (HALF_QUOTED);
	}
	else
		return (NO_QUOTED);
}
