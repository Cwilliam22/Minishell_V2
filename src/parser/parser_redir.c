/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:51:17 by alexis            #+#    #+#             */
/*   Updated: 2025/07/20 15:30:02 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Convert token type to redirection type
 * @param token_type: Token type from lexer
 * @return: Corresponding redirection type
 */
static t_redir_type	convert_token_to_redir(t_token_type token_type)
{
	if (token_type == T_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == T_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == T_APPEND)
		return (REDIR_APPEND);
	else if (token_type == T_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

/**
 * Extract redirections from token list
 * @param tokens: Starting token
 * @param cmd: Command to fill
 * @return: 1 on success, 0 on error
 */
int	extract_redirections(t_token *tokens, t_cmd *cmd)
{
	t_token			*current;
	t_redir			*redir;
	char			*filename;

	current = tokens;
	while (current && current->type != T_PIPE)
	{
		if (current->type >= T_REDIR_IN && current->type <= T_HEREDOC)
		{
			if (!current->next || (current->next->type != T_WORD
					&& current->next->type != T_VAR))
				return (0);
			filename = ft_strdup(current->next->value);
			if (!filename)
				return (0);
			redir = create_redirection(convert_token_to_redir(current->type),
					filename);
			free(filename);
			if (!redir)
				return (0);
			add_redirection(&cmd->redirections, redir);
			current = current->next;
		}
		current = current->next;
	}
	return (1);
}
