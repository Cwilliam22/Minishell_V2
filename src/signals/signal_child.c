/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:11:03 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:11:26 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child_sigint(int sig)
{
	set_exit_status(sig);
	g_signal_received = sig;
	cleanup_and_exit(130, NULL);
}

void	heredoc_child_signal(void)
{
	signal(SIGINT, heredoc_child_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	sigint_handler_child(int sig)
{
	write (1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	set_exit_status(sig);
	g_signal_received = sig;
}
