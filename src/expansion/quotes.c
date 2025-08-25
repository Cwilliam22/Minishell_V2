/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:34:36 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:34:36 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Process quoted sections and handle variable expansion
 * @param str: Input string
 * @param shell: Shell structure
 * @return: Processed string without quotes
 */
char	*handle_quotes(char *str, t_shell *shell)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			result = process_quoted_section(result, str, &i, shell);
		else
			result = process_unquoted_char(result, str, &i, shell);
	}
	return (result);
}

static int	handle_double_quotes(char *str, int *i)
{
	(*i)++;
	while (str[*i] && str[*i] != '"')
		(*i)++;
	if (!str[*i])
		return (0);
	if (str[*i] && str[*i] == '"')
	{
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_single_quotes(char *str, int *i)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (!str[*i])
		return (0);
	if (str[*i] && str[*i] == '\'')
	{
		(*i)++;
		return (1);
	}
	return (0);
}

/**
 * @brief Validates quote syntax in string
 * @param str String to validate
 * @return 0 if quotes are valid, -1 on error
 */
int	check_quotes(char *str)
{
	int	i;

	if (!str || !str[0])
		return (1);
	i = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
		i++;
	if (str[i] == '"')
	{
		if (!handle_double_quotes(str, &i))
			return (0);
		return (check_quotes(str + i));
	}
	else if (str[i] == '\'')
	{
		if (!handle_single_quotes(str, &i))
			return (0);
		return (check_quotes(str + i));
	}
	return (1);
}
