/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:50:02 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/24 15:38:16 by alexis           ###   ########.fr       */
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
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	handle_redirection_only(t_cmd *cmd, t_exec *exec)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	pid = fork();
	if (pid == 0)
	{
		child_signal();
		exit_code = apply_redirections(cmd);
		set_exit_status(exit_code);
		cleanup_and_exit(exit_code, exec);
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
