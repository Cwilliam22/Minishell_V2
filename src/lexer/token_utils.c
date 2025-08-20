/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:45:49 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/20 11:46:48 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Create a new token
 * @param value: Token value
 * @param type: Token type
 * @return: New token or NULL on error
 */
t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = (t_token *)safe_malloc(sizeof(t_token));
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

/**
 * Add token to the end of the list
 * @param head: Pointer to the head of the token list
 * @param new_token: Token to add
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/**
 * Free all tokens in the list
 * @param tokens: Head of the token list
 */
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

int	check_token_quotes(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->type == T_WORD || current->type == T_VAR)
		{
			if (!check_quotes(current->value))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
