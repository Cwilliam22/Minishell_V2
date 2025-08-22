/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:53:09 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 11:55:39 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_child_signal(void)
{
	signal(SIGINT, SIG_DFL);
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
	g_signal_received = sig;
}
