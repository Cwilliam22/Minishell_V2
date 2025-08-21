/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:48:51 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 23:41:11 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_single_redirection(t_redir *curr)
{
	int	ret;

	if (curr->type == REDIR_IN)
		ret = open_and_dup(curr->file, O_RDONLY, STDIN_FILENO);
	else if (curr->type == REDIR_OUT)
		ret = open_and_dup(curr->file, O_WRONLY | O_CREAT
				| O_TRUNC, STDOUT_FILENO);
	else if (curr->type == REDIR_APPEND)
		ret = open_and_dup(curr->file, O_WRONLY | O_CREAT
				| O_APPEND, STDOUT_FILENO);
	else
		ret = 1;
	return (ret);
}

int	apply_redirections(t_cmd *cmd)
{
	t_redir	*curr;

	if (!cmd || !cmd->redirections)
		return (0);
	curr = cmd->redirections;
	while (curr)
	{
		if (apply_single_redirection(curr) != 0)
			return (-1);
		curr = curr->next;
	}
	return (0);
}

void	handle_redirection_only(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		apply_redirections(cmd);
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		update_exit_status_from_child(status);
	}
	else
	{
		perror("fork");
		set_exit_status(1);
	}
}
