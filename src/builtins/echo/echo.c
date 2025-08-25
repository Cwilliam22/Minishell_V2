/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:37:27 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:37:27 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_print_string(char *arg, t_shell *g_shell)
{
	char	*home;

	if (ft_strcmp(arg, "~") == 0)
	{
		home = get_env_var("HOME", g_shell->env);
		return (home);
	}
	return (arg);
}

static void	print_argument(char *to_print, int is_home)
{
	write(STDOUT_FILENO, to_print, ft_strlen(to_print));
	if (is_home)
		free(to_print);
}

static int	ft_printf_arg(char **tab_arg, int index, int option)
{
	int		i;
	t_shell	*g_shell;
	char	*to_print;
	int		is_home;

	g_shell = get_shell(NULL);
	i = index;
	while (tab_arg[i])
	{
		is_home = (ft_strcmp(tab_arg[i], "~") == 0);
		to_print = get_print_string(tab_arg[i], g_shell);
		print_argument(to_print, is_home);
		if (tab_arg[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (option == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
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
