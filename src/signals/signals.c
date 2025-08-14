/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:18:10 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/14 12:18:10 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(void)
{
	if (g_signal_received == SIGINT)
		set_exit_status(EXIT_SIGINT);
	g_signal_received = 0;
}

void	handle_sigint(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal_received = sig;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
}

void	handle_signal_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(EXIT_SIGINT);
}
