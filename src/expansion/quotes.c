/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:19:22 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/20 12:24:21 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Extract variable name from string and update index
 * @param str: String starting at variable name
 * @param index: Pointer to current index (updated)
 * @return: Variable name or NULL on error
 */
char	*extract_var_name(char *str, int *index)
{
	char	*var_name;
	int		len;

	if (!str || !index)
		return (NULL);
	if (str[0] == '?')
		return (*index += 2, ft_strdup("?"));
	if (ft_isdigit(str[0]))
	{
		*index += 2;
		var_name = ft_substr(str, 0, 1);
		return (var_name);
	}
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (ft_strdup(""));
	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	*index += len + 1;
	var_name = ft_substr(str, 0, len);
	return (var_name);
}

static char	*process_variable(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*result;

	if (str [*i + 1] == '?')
		return ((*i) += 2, ft_itoa(shell->env->last_exit_status));
	var_name = extract_var_name(str + *i + 1, i);
	if (!var_name || var_name[0] == '\0')
		return ((*i)++, ft_strdup("$"));
	var_value = get_env_var(var_name, shell->env);
	if (var_value && ft_strlen(var_value) > 0)
		result = var_value;
	else
	{
		result = ft_strdup("");
		free(var_value);
	}
	free(var_name);
	return (result);
}

/**
 * Expand variables in a string
 * @param str: Input string
 * @param shell: Shell structure
 * @return: New string with variables expanded
 */
char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			temp = process_variable(str, &i, shell);
			result = join_and_free(result, temp);
		}
		else
			result = append_char_to_str(result, str[i++]);
	}
	return (result);
}

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
	if (ft_strcmp(result, "") == 0)
		return (NULL);
	return (result);
}

/**
 * Check if quotes are properly balanced
 * @param str: String to check
 * @return: 1 if balanced, 0 otherwise
 */
int check_quotes(char *str)
{
	int len, i;
	char first_char, last_char;
	int single_count = 0, double_count = 0;
	
	if (!str || !str[0])
		return (1);
	len = strlen(str);
	first_char = str[0];
	last_char = str[len - 1];
	if ((first_char == '\'' || first_char == '"') && last_char == first_char)
	{
		i = 1;
		while (i < len - 1)
		{
			if (str[i++] == first_char)
				break;
		}
		if (i == len - 1)
			return (1);
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			single_count++;
		else if (str[i] == '"')
			double_count++;
		i++;
	}
	return (single_count % 2 == 0 && double_count % 2 == 0);
}
