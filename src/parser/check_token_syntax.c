/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:48:37 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/20 11:49:20 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Print pipe syntax error message
 * @param token: Token that caused the error
 */
static int	check_pipe_syntax(t_token *current)
{
	if (!current->next || current->next->type == T_PIPE)
		return (print_syntax_error("|"), SYNTAX_ERROR);
	return (1);
}

/**
 * Print redir syntax error message
 * @param token: Token that caused the error
 */
static int	check_redir_syntax(t_token *current)
{
	if (!current->next || (current->next->type != T_WORD && current->next->type != T_VAR))
	{
		if (current->next)
			print_syntax_error(current->next->value);
		else
			print_syntax_error("newline");
		return (SYNTAX_ERROR);
	}
	return (1);
}

/**
 * Check syntax errors in token list
 * @param tokens: Head of token list
 * @return: 1 if syntax is valid, the value of the synthax error otherwise
 */
int	check_token_syntax(t_token *tokens)
{
	t_token	*current;
	int		result;

	if (!tokens)
		return (1);
	current = tokens;
	result = 1;
	if (check_token_quotes(tokens) == 0)
		return (print_syntax_error("unclosed quotes"), SYNTAX_ERROR);
	if (current->type == T_PIPE)
		return (print_syntax_error("|"), SYNTAX_ERROR);
	while (current)
	{
		if (current->type == T_PIPE)
			result = check_pipe_syntax(current);
		else if (current->type >= T_REDIR_IN
			&& current->type <= T_HEREDOC)
			result = check_redir_syntax(current);
		if (result != 1)
			return (result);
		current = current->next;
	}
	return (1);
}
