/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:04:38 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 03:48:47 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_assignment(char *arg, t_cmd *cmd)
{
	char	*key;
	char	*value;
	t_ass	*assign;

	assign = NULL;
	if (is_append_assignment_word(arg)
		&& split_append_assignment(arg, &key, &value))
	{
		assign = create_append_assignment(key, value);
		if (assign)
			add_assignment(&cmd->assignments, assign);
		free(key);
		free(value);
	}
	else if (is_assignment_word(arg) && split_assignment(arg, &key, &value))
	{
		assign = create_assignment(key, value);
		if (assign)
			add_assignment(&cmd->assignments, assign);
		free(key);
		free(value);
	}
}

static int	process_word_token(t_token *current, t_cmd *cmd, int *i)
{
	int		j;
	char	*processed_value;

	processed_value = ft_strdup(current->value);
	if (!processed_value)
	{
		j = *i;
		while (--j >= 0)
			free(cmd->args[j]);
		return (free(cmd->args), cmd->args = NULL, 0);
	}
	cmd->args[*i] = processed_value;
	if (*i > 0 && cmd->args[0] && (ft_strcmp(cmd->args[0], "export") == 0
			|| ft_strcmp(cmd->args[0], "declare") == 0))
		handle_assignment(cmd->args[*i], cmd);
	return ((*i)++, 1);
}

static int	init_and_check_args(t_cmd *cmd, int word_count)
{
	if (word_count == 0)
		return (cmd->args = NULL, 1);
	cmd->args = (char **)safe_malloc(sizeof(char *) * (word_count + 1));
	return (1);
}

int	extract_arguments(t_token *tokens, t_cmd *cmd)
{
	t_token	*curr;
	int		word_count;
	int		i;
	int		skip_next;

	word_count = count_words(tokens);
	if (!init_and_check_args(cmd, word_count) || word_count == 0)
		return (word_count == 0);
	curr = tokens;
	i = 0;
	skip_next = 0;
	while (curr && curr->type != T_PIPE && i < word_count)
	{
		if (skip_next)
			skip_next = 0;
		else if (curr->type == T_WORD || curr->type == T_VAR)
		{
			if (!process_word_token(curr, cmd, &i))
				return (0);
		}
		else if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC)
			skip_next = 1;
		curr = curr->next;
	}
	return (cmd->args[i] = NULL, 1);
}
