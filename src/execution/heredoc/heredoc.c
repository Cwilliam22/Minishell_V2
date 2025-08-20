/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:23:35 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/20 16:24:12 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_hd_var(char *line, int *i, t_env *env, char *result)
{
	int		start;
	char	*var_name;
	char	*value;

	start = *i;
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	var_name = ft_substr(line, start, *i - start);
	value = get_env_var(var_name, env);
	if (!value)
		return (free(var_name), NULL);
	result = join_and_free(result, value);
	return (result);
}

char	*dollar_case(char *line, int *i, char *result)
{
	char	*temp;

	(*i)++;
	if (line[*i] == '?')
	{
		temp = ft_itoa(get_shell(NULL)->env->last_exit_status);
		if (!temp)
			return (NULL);
		result = join_and_free(result, temp);
		(*i)++;
	}
	else if (ft_isalpha(line[*i]) || line[*i] == '_')
		result = handle_hd_var(line, i, get_shell(NULL)->env, result);
	else
		result = join_and_free(result, "$");
	return (result);
}

char	*expand_vars(char *line)
{
	int		i;
	char	*result;

	i = 0;
	result = NULL;
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

void	handle_heredoc(t_cmd *cmds)
{
	t_cmd	*current;
	t_redir	*redir;

	if (!cmds)
		return ;
	current = cmds;
	while (current)
	{
		if (current->redirections
			&& current->redirections->type == REDIR_HEREDOC)
		{
			redir = current->redirections;
			if (!setup_for_heredoc(redir->heredoc))
				return ;
			if (!create_file(redir))
				return ;
			process_hd(redir);
		}
		current = current->next;
	}
}
