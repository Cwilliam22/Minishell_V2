/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:35:18 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/17 11:35:18 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static ssize_t	ft_atoll(char *str)
{
	int		i;
	int		sign;
	ssize_t	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	ft_is_a_number(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !*arg)
		return (0);
	while (is_whitespace(arg[i]))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i] && ft_isdigit((int)arg[i]))
		i++;
	while (is_whitespace(arg[i]))
		i++;
	return (arg[i] == '\0');
}

static int	handle_single_arg(char *arg)
{
	ssize_t	exit_code;

	if (!ft_is_a_number(arg) || !check_limit(arg))
	{
		print_error("exit", arg, "numeric argument required");
		return (2);
	}
	exit_code = ft_atoll(arg);
	return ((exit_code % 256 + 256) % 256);
}

int	builtin_exit(t_exec *exec)
{
	char	**args;
	ssize_t	exit_code;

	args = exec->shell->commands->args_expanded;
	printf("exit\n");
	if (!args[1])
		exit_code = exec->shell->env->last_exit_status;
	else if (args[2])
	{
		print_error("exit", NULL, "too many arguments");
		return (1);
	}
	else
		exit_code = handle_single_arg(args[1]);
	cleanup_all(exec);
	exit(exit_code);
}
