/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:01:46 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:01:50 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Prints syntax error message for invalid token
* @param token Invalid token that caused the error
*/
void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token", 2);
	if (token)
	{
		ft_putstr_fd(" `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("\'", 2);
	}
	ft_putstr_fd("\n", 2);
}

/**
* @brief Prints formatted error message
* @param cmd Command that caused the error
* @param arg Argument related to the error (can be NULL)
* @param msg Error message to display
*/
void	print_error(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		if (arg || msg)
			ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		if (msg)
			ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}
