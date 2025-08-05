/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 01:32:04 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 01:43:21 by alexis           ###   ########.fr       */
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
		if (strncmp("-n", arg[i], 2) == 0)
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
		printf("%s", tab_arg[i]);
		if (tab_arg[i + 1])
			printf(" ");
		i++;
	}
	if (option == 0)
		printf("\n");
	return (1);
}

int	builtin_echo(t_exec *exec)
{
	int		i;
	char	**arg;

	i = 1;
	arg = exec->shell->commands->args;
	if (arg[1] == NULL)
	{
		printf("\n");
		return (0);
	}
	else if (ft_strncmp("-n", arg[1], 2) == 0)
	{
		i = skip_n(arg);
		if (i == -1)
			return (1);
		if (!ft_printf_arg(arg, i, 1))
			return (1);
	}
	else
	{
		if (!ft_printf_arg(arg, i, 0))
			return (1);
	}
	return (0);
}
