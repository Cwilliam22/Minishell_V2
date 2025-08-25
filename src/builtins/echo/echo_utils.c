/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:48:30 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:48:36 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (i == 1)
		return (0);
	return (arg[i] == '\0' || arg[i] == ' ' || arg[i] == '\t');
}

/**
 * @brief Skips all consecutive -n flags in arguments array.
 * 
 * @param args Array of command arguments
 * @return int Index of first non -n flag argument
 */
int	skip_all_n_flags(char **args)
{
	int	i;

	i = 1;
	while (args[i] && is_valid_n_flag(args[i]))
		i++;
	return (i);
}

/**
 * @brief Prints variable assignments to stdout.
 * 
 * @param assignments Linked list of variable assignments
 */
void	print_assignments(t_ass *assignments)
{
	t_ass	*head;

	if (!assignments)
		return ;
	head = assignments;
	while (head)
	{
		write(STDOUT_FILENO, head->key, ft_strlen(head->key));
		write(STDOUT_FILENO, "=", 1);
		write(STDOUT_FILENO, head->value, ft_strlen(head->value));
		write(STDOUT_FILENO, " ", 1);
		head = head->next;
	}
}
