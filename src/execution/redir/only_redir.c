/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:48:16 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/17 11:49:21 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_empty_file(char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (1);
	}
	close(fd);
	return (0);
}

static int	has_input_redirection(t_redir *redir)
{
	t_redir	*head;

	head = redir;
	while (head)
	{
		if (head->type == REDIR_IN)
			return (1);
		head = head->next;
	}
	return (0);
}

static int	has_output_redirection(t_redir *redir)
{
	t_redir	*head;

	head = redir;
	while (head)
	{
		if (head->type == REDIR_OUT)
			return (1);
		head = head->next;
	}
	return (0);
}

void	handle_redirection_only(t_cmd *cmd)
{
	if (has_output_redirection(cmd->redirections))
	{
		if (create_empty_file(cmd->redirections->file) != SUCCESS)
		{
			set_exit_status(1);
			print_error(NULL, NULL, "error open empty file");
		}
	}
	if (has_input_redirection(cmd->redirections))
	{
		if (!check_command_exist(cmd->redirections->file))
		{
			set_exit_status(1);
			print_error(NULL, NULL, "so such file or directory");
			return ;
		}
	}
}
