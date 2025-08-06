/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:46:15 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/06 14:50:50 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_nb_command_args(t_cmd *commands)
{
	char	**args;
	int		i;

	args = commands->args;
	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}
