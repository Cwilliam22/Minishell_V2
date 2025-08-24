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

static int	is_valid_n_flag(char *arg, int is_quoted)
{
	int		i;
	char	*trimmed;

	i = 0;
	if (!arg || arg[0] != '-')
		return (0);
	if (!is_quoted)
		trimmed = ft_strtrim(arg, " ");
	else
		trimmed = ft_strdup(arg);
	i = 1;
	while (trimmed[i] == 'n')
		i++;
	if (trimmed[i] != '\0' && isspace(trimmed[i]))
		return (free(trimmed), 0);
	free(trimmed);
	return (i > 1);
}

static int	skip_all_n_flags(char **args, int is_quoted)
{
	int	i;

	i = 1;
	while (args[i] && is_valid_n_flag(args[i], is_quoted) && !is_quoted)
		i++;
	return (i);
}

static int	ft_printf_arg(char **tab_arg, int index, int option)
{
	int		i;
	t_shell	*g_shell;
	char	*to_print;
	char	*home;

	g_shell = get_shell(NULL);
	i = index;
	home = NULL;
	while (tab_arg[i])
	{
		if (ft_strcmp(tab_arg[i], "~") == 0)
		{
			home = get_env_var("HOME", g_shell->env);
			to_print = home;
		}
		else
			to_print = tab_arg[i];
		write(STDOUT_FILENO, to_print, ft_strlen(to_print));
		if (home)
		{
			free(home);
			home = NULL;
		}
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
	int		is_quoted;

	suppress_newline = 0;
	is_quoted = 0;
	arg = exec->current_cmd->args_expanded;
	if (arg[1] == NULL)
	{
		if (exec->current_cmd->assignments)
			print_assignments(exec->current_cmd->assignments);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	int j = 0;
	if (exec->current_cmd->args[0] && exec->current_cmd->args[0][0] == '$')
		is_quoted = 0;
	else if (exec->current_cmd->args[1])
	{
		while (exec->current_cmd->args[1][j] && exec->current_cmd->args[1][j] != '"')
			j++;
		if (exec->current_cmd->args[1][j] == '"')
			is_quoted = 1;
	}
	i = skip_all_n_flags(arg, is_quoted);
	if (i > 1)
		suppress_newline = 1;
	if (!ft_printf_arg(arg, i, suppress_newline))
		return (1);
	return (0);
}
