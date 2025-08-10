/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_assignments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:51:21 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/10 13:05:09 by alfavre          ###   ########.fr       */
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

int	extract_assignments(t_token *tokens, t_cmd *cmd)
{
	t_token	*curr;

	curr = tokens;
	while (curr && curr->type != T_PIPE)
	{
		if (curr->type == T_ASS)
			handle_assignment(curr->value, cmd);
		curr = curr->next;
	}
	return (1);
}
