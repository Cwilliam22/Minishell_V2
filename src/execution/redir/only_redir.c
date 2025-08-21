/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcapt < wcapt@student.42lausanne.ch >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:48:51 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 12:20:25 by wcapt            ###   ########.fr       */
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

static void	condition_handle_redirection_only(t_cmd *cmd, t_redir *head)
{
	if (head->type == REDIR_IN)
	{
		if (!check_command_exist(cmd->redirections->file))
		{
			set_exit_status(1);
			print_error(NULL, NULL, "so such file or directory");
			return ;
		}
	}
	else if (head->type == REDIR_OUT || head->type == REDIR_APPEND)
	{
		if (create_empty_file(cmd->redirections->file) != SUCCESS)
		{
			set_exit_status(1);
			print_error(NULL, NULL, "error open empty file");
		}
	}
}

void	handle_redirection_only(t_cmd *cmd)
{
	t_redir	*head;

	if (!cmd || !cmd->redirections)
		return ;
	head = cmd->redirections;
	while (head)
	{
		condition_handle_redirection_only(cmd, head);
		head = head->next;
	}
}
