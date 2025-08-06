/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:54:22 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/06 13:23:13 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_limit(char *str)
{
	char	*tmp;
	int		neg;

	tmp = str;
	neg = 0;
	while (is_whitespace(*tmp))
		tmp++;
	if (*tmp == '-' || *tmp == '+')
	{
		if (*tmp == '-')
		{
			neg = 1;
		}
		tmp++;
	}
	if (ft_strlen(tmp) > 19
		|| (neg && (ft_strcmp("9223372036854775808", tmp) < 0))
		|| (!neg && (ft_strcmp("9223372036854775807", tmp)) < 0))
		return (0);
	return (1);
}

static ssize_t	ft_atoll(char *str)
{
	int		i;
	int		sign;
	ssize_t	result;
	ssize_t	prev;

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
		prev = result;
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
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit((int)arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_single_arg(char *arg)
{
	ssize_t	exit_code;

	if (ft_is_a_number(arg))
	{
		if (check_limit(arg))
		{
			exit_code = ft_atoll(arg);
			return ((exit_code % 256 + 256) % 256);
		}
		else
		{
			print_error("exit", arg, "numeric argument required");
			return (2);
		}
	}
	else
	{
		print_error("exit", arg, "numeric argument required");
		return (2);
	}
}

int	builtin_exit(t_exec *exec)
{
	char	**args;
	ssize_t	exit_code;

	args = exec->shell->commands->args;
	printf("exit\n");
	if (args[1])
	{
		if (!ft_is_a_number(args[1]))
		{
			print_error("exit", args[1], "numeric argument required");
			exit_code = 2;
		}
		else if (args[2])
		{
			print_error("exit", NULL, "too many arguments");
			return (1);
		}
		else
			exit_code = handle_single_arg(args[1]);
	}
	else
		exit_code = exec->shell->env->last_exit_status;
	cleanup_all(exec);
	exit(exit_code);
}
