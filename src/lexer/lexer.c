/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:33:44 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:33:44 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_assignment(char *str)
{
	char	*ptr;
	char	save;
	int		valid;

	ptr = ft_strchr(str, '=');
	if (ptr && ptr != str)
	{
		save = *ptr;
		*ptr = '\0';
		valid = is_a_valid_identifier(str);
		*ptr = save;
		if (valid)
			return (T_ASS);
	}
	return (T_WORD);
}

static int	get_token_type(char *str)
{
	if (!str)
		return (T_EOF);
	if (ft_strcmp(str, "|") == 0)
		return (T_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (T_REDIR_IN);
	if (ft_strcmp(str, ">") == 0)
		return (T_REDIR_OUT);
	if (ft_strcmp(str, ">>") == 0)
		return (T_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (T_HEREDOC);
	if (str[0] == '$' && str[1] != '\0')
		return (T_VAR);
	if (str[0] == '\0')
		return (T_EOF);
	return (is_assignment(str));
}

static t_token	*create_and_add_token(char *token_value, t_token **head)
{
	t_token	*new_token;

	new_token = create_token(token_value, get_token_type(token_value));
	if (!new_token)
		return (NULL);
	add_token(head, new_token);
	return (new_token);
}

static char	*extract_next_token(char *input, int i, int *end)
{
	char	*op;

	if (is_operator(input[i]))
	{
		op = extract_operator(input, i, end);
		if (!op)
		{
			print_syntax_error(NULL);
			set_exit_status(2);
			return (NULL);
		}
	}
	else
	{
		op = extract_word(input, i, end);
	}
	return (op);
}

/**
 * Main tokenization function
 * @param input: Input string to tokenize
 * @return: Head of token list or NULL on error
 */
t_token	*tokenize(char *input)
{
	t_token	*head;
	char	*token_value;
	int		i;
	int		end;

	if (!input)
		return (NULL);
	head = NULL;
	i = 0;
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break ;
		token_value = extract_next_token(input, i, &end);
		if (!token_value || !create_and_add_token(token_value, &head))
			return (free(token_value), free_tokens(head), NULL);
		free(token_value);
		i = end;
	}
	return (head);
}
