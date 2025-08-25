/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_parent.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:13:33 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:13:33 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int sig)
{
	write (1, "\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal_received = sig;
	set_exit_status(sig);
}

static void	sigquit_handler(int sig)
{
	(void)sig;
	ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
}

void	parent_signal(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_parent_signal(void)
{
	signal(SIGINT, sigint_handler_child);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_core_dump_parent_signal(void)
{
	signal(SIGINT, sigint_handler_child);
	signal(SIGQUIT, sigquit_handler);
}
