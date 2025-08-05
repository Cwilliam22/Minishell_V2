/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:05:59 by alfavre           #+#    #+#             */
/*   Updated: 2025/07/18 14:45:46 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Count the number of word tokens (arguments) before pipe or end
 * @param tokens: Token list to analyze
 * @return: Number of word tokens found
 */
int	count_words(t_token *tokens)
{
	t_token	*current;
	int		count;
	int		skip_next;

	if (!tokens)
		return (0);
	current = tokens;
	count = 0;
	skip_next = 0;
	while (current && current->type != T_PIPE)
	{
		if (skip_next)
			skip_next = 0;
		else if (current->type == T_WORD || current->type == T_VAR)
			count++;
		else if (current->type >= T_REDIR_IN
			&& current->type <= T_HEREDOC)
			skip_next = 1;
		current = current->next;
	}
	return (count);
}
