/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:34:52 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 20:36:29 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_leading_zeros(char *str)
{
	while (*str == '0')
		str++;
	if (*str == '\0')
		str--;
	return (str);
}

int	check_limit(char *str)
{
	char	*tmp;
	int		neg;
	char	*after_zeros;

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
	after_zeros = skip_leading_zeros(tmp);
	if (ft_strlen(after_zeros) > 19
		|| (neg && (ft_strcmp("9223372036854775808", tmp) < 0))
		|| (!neg && (ft_strcmp("9223372036854775807", tmp)) < 0))
		return (0);
	return (1);
}
