/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:17:10 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 00:30:17 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_hd_var(char *line, int *i, t_env *env, char *result)
{
	int		start;
	char	*var_name;
	char	*value;

	start = *i;
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	var_name = ft_substr(line, start, *i - start);
	value = get_env_var(var_name, env);
	free(var_name);
	if (value)
		result = join_and_free(result, value);
	return (result);
}

static char	*dollar_case(char *line, int *i, char *result)
{
	char	*temp;
	t_shell	*shell;

	shell = get_shell(NULL);
	(*i)++;
	if (line[*i] == '?')
	{
		temp = ft_itoa(shell->env->last_exit_status);
		if (!temp)
			return (NULL);
		result = join_and_free(result, temp);
		(*i)++;
	}
	else if (ft_isalpha(line[*i]) || line[*i] == '_')
		result = handle_hd_var(line, i, shell->env, result);
	else
		result = join_and_free(result, "$");
	return (result);
}

static char	*expand_vars(char *line)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (line[i])
	{
		if (line[i] == '$')
		{
			result = dollar_case(line, &i, result);
		}
		else
		{
			result = append_char_to_str(result, line[i]);
			i++;
		}
	}
	return (result);
}

char	*expand_heredoc_content(t_redir *redir, char *line)
{
	char	*new_line;

	if (!redir || !redir->heredoc || !line)
		return (NULL);
	if (redir->heredoc->quoted_delimiter)
		new_line = ft_strdup(line);
	else
		new_line = expand_vars(line);
	return (new_line);
}
