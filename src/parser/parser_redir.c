/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:07:04 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 14:07:10 by alfavre          ###   ########.ch       */
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

static int	is_valid_redirection_target(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == T_WORD || token->type == T_VAR)
		return (1);
	return (0);
}

static t_redir	*create_redir_from_token(t_token *current, t_cmd *cmd)
{
	char	*filename;
	t_redir	*redir;

	filename = ft_strdup(current->next->value);
	if (!filename)
		return (NULL);
	redir = create_redirection(convert_token_to_redir(current->type),
			filename);
	if (redir && redir->type == REDIR_HEREDOC)
		redir->heredoc->id = cmd->nb_hd++;
	free(filename);
	return (redir);
}

static int	process_redirection(t_token **current, t_cmd *cmd)
{
	t_redir	*redir;

	if (!is_valid_redirection_target((*current)->next))
		return (0);
	redir = create_redir_from_token(*current, cmd);
	if (!redir)
		return (0);
	add_redirection(&cmd->redirections, redir);
	*current = (*current)->next;
	return (1);
}

int	extract_redirections(t_token *tokens, t_cmd *cmd)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != T_PIPE)
	{
		if (current->type >= T_REDIR_IN && current->type <= T_HEREDOC)
		{
			if (!process_redirection(&current, cmd))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
