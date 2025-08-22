/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:43:46 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 09:43:51 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

static int	skip_all_n_flags(char **args)
{
	int	i;

	i = 1;
	while (args[i] && is_valid_n_flag(args[i]))
		i++;
	return (i);
}

static int	ft_printf_arg(char **tab_arg, int index, int option)
{
	int		i;
	t_shell	*g_shell;

	g_shell = get_shell(NULL);
	i = index;
	while (tab_arg[i])
	{
		if (ft_strcmp(tab_arg[i], "~") == 0)
			tab_arg[i] = get_env_var("HOME", g_shell->env);
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
	int		suppress_newline;

	suppress_newline = 0;
	arg = exec->current_cmd->args_expanded;
	if (arg[1] == NULL)
	{
		if (exec->current_cmd->assignments)
			print_assignments(exec->current_cmd->assignments);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	i = skip_all_n_flags(arg);
	if (i > 1)
		suppress_newline = 1;
	if (!ft_printf_arg(arg, i, suppress_newline))
		return (1);
	return (0);
}
