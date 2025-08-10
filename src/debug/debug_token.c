/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:35:32 by alexis            #+#    #+#             */
/*   Updated: 2025/07/18 14:59:26 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Debug function to print tokens
 * @param tokens: Head of token list
 */
void	print_tokens(t_token *tokens)
{
	t_token		*current;
	const char	*type_names[] = {
		"T_WORD", "T_PIPE", "T_REDIR_IN", "T_REDIR_OUT", "T_APPEND",
		"T_HEREDOC", "T_EOF", "T_VAR", "T_ASS"
	};

	current = tokens;
	printf("=== TOKENS ===\n");
	while (current)
	{
		printf("Type: %-12s Value: '%s'\n",
			type_names[current->type], current->value);
		current = current->next;
	}
	printf("==============\n");
}
