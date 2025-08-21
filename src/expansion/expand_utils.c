/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:56:36 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 13:56:36 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*handle_backslash(char *result, char *str, int *index)
{
	if (str[*index + 1] == '\\')
	{
		result = append_char_to_str(result, '\\');
		*index += 2;
	}
	else if (str[*index + 1] == '$')
	{
		result = append_char_to_str(result, '$');
		*index += 2;
	}
	else
	{
		result = append_char_to_str(result, str[*index]);
		(*index)++;
	}
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
		if (str[i] == '\\' && str[i + 1])
			result = handle_backslash(result, str, &i);
		else if (str[i] == '$' && str[i + 1])
		{
			temp = process_variable(str, &i, shell);
			result = join_and_free(result, temp);
		}
		else
			result = append_char_to_str(result, str[i++]);
	}
	return (result);
}
