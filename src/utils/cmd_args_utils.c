/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:59:00 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:59:00 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Counts the number of arguments in a command
* @param commands Command structure containing arguments
* @return Number of arguments found
*/
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
