/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:44:26 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 14:44:26 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_output_file(char *filename)
{
	int	fd;

	if (!filename || !*filename)
		return (0);
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == EACCES)
			print_error(NULL, filename, "Permission denied");
		else if (errno == ENOENT)
			print_error(NULL, filename, "No such file or directory");
		else if (errno == EISDIR)
			print_error(NULL, filename, "Is a directory");
		else
			print_error(NULL, filename, strerror(errno));
		return (1);
	}
	close(fd);
	return (0);
}

static int	validate_single_redirection(t_redir *redir)
{
	if (redir->type == REDIR_IN)
	{
		if (access(redir->file, F_OK) == -1)
		{
			print_error(NULL, redir->file, "No such file or directory");
			set_exit_status(1);
			return (-1);
		}
	}
	else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
	{
		if (validate_output_file(redir->file) == 0)
		{
			set_exit_status(1);
			return (-1);
		}
	}
	return (0);
}

int	validate_file_redirections(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmds;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (validate_single_redirection(current_redir) == -1)
				return (-1);
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

int	has_file_redirections(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmds;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (current_redir->type == REDIR_IN
				|| current_redir->type == REDIR_OUT
				|| current_redir->type == REDIR_APPEND)
				return (1);
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}
