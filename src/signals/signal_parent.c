/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_parent.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:09:19 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 16:09:19 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_parent_signal(void)
{
	signal(SIGINT, handler_child_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_core_dump_parent_signal(void)
{
	signal(SIGINT, handler_child_sigint);
	signal(SIGQUIT, handle_sigquit);
}
