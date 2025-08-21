/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:40:24 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 14:41:10 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_n(char **arg)
{
	int	i;
	int	j;

	i = 1;
	j = 1;
	if (!arg)
		return (-1);
	while (arg[j])
	{
		if (ft_strncmp("-n", arg[i], 2) == 0)
			i++;
		j++;
	}
	return (i);
}

static int	ft_printf_arg(char **tab_arg, int index, int option)
{
	int	i;

	i = index;
	while (tab_arg[i])
	{
		write(STDOUT_FILENO, tab_arg[i], ft_strlen(tab_arg[i]));
		if (tab_arg[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (option == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}

static void	print_assignments(t_ass *assignments)
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

int	builtin_echo(t_exec *exec)
{
	int		i;
	char	**arg;

	i = 1;
	arg = exec->current_cmd->args_expanded;
	if (arg[1] == NULL)
	{
		if (exec->current_cmd->assignments)
			print_assignments(exec->current_cmd->assignments);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	if (ft_strncmp("-n", arg[1], 2) == 0)
	{
		i = skip_n(arg);
		if (i == -1 || !ft_printf_arg(arg, i, 1))
			return (1);
	}
	else
	{
		if (!ft_printf_arg(arg, i, 0))
			return (1);
	}
	return (0);
}
