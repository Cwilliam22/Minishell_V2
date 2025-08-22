/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:19:24 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 11:19:24 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(void)
{
	if (g_signal_received == SIGINT)
		set_exit_status(130);
	g_signal_received = 0;
}
