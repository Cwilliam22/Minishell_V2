/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:36:37 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/17 11:36:37 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Get string representation of redirection type
 * @param type: Redirection type
 * @return: String representation
 */
static void	print_args(char **args)
{
	int	i;

	if (args)
	{
		printf("  Args: ");
		i = 0;
		while (args[i])
		{
			printf("'%s' ", args[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("  No args\n");
}

/**
 * Get string representation of redirection type
 * @param type: Redirection type
 * @return: String representation
 */
static void	print_assignments(t_ass *assignments)
{
	t_ass	*assign;

	if (assignments)
	{
		printf("  Assignments:\n");
		assign = assignments;
		while (assign)
		{
			printf("    %s = %s\n", assign->key, assign->value);
			assign = assign->next;
		}
	}
}

/**
 * Get string representation of redirection type
 * @param type: Redirection type
 * @return: String representation
 */
static void	print_redirections(t_redir *redirections)
{
	t_redir	*redir;

	if (!redirections)
		return ;
	printf("  Redirections:\n");
	redir = redirections;
	while (redir)
	{
		printf("    %s -> ", get_redir_types(redir->type));
		if (redir->type == REDIR_HEREDOC && redir->heredoc)
		{
			printf("delimiter:'%s'", redir->heredoc->delimiter);
			if (redir->heredoc->quoted_delimiter)
				printf(" (quoted)");
		}
		else if (redir->file)
			printf("'%s'", redir->file);
		else
			printf("(no target)");
		if (redir->fd_in != -1 || redir->fd_out != -1)
			printf(" (fd_in: %d, fd_out: %d)", redir->fd_in, redir->fd_out);
		printf("\n");
		redir = redir->next;
	}
}

/**
 * Debug function to print command structure
 * @param commands: Head of command list
 */
void	print_commands(t_cmd *commands)
{
	t_cmd	*current;
	int		cmd_num;

	current = commands;
	cmd_num = 1;
	printf("=== COMMANDS ===\n");
	while (current)
	{
		printf("Command %d:\n", cmd_num);
		print_args(current->args);
		print_assignments(current->assignments);
		print_redirections(current->redirections);
		if (current->next)
			printf("  -> PIPE TO NEXT COMMAND\n");
		printf("\n");
		current = current->next;
		cmd_num++;
	}
	printf("================\n");
}

/**
 * Debug function to print command structure
 * @param commands: Head of command list
 */
void	print_commands_expanded(t_cmd *commands)
{
	t_cmd	*current;
	int		cmd_num;

	current = commands;
	cmd_num = 1;
	printf("=== COMMANDS EXPANDED ===\n");
	while (current)
	{
		printf("Command %d:\n", cmd_num);
		print_args(current->args_expanded);
		print_assignments(current->assignments);
		print_redirections(current->redirections);
		if (current->next)
			printf("  -> PIPE TO NEXT COMMAND\n");
		printf("\n");
		current = current->next;
		cmd_num++;
	}
	printf("================\n");
}
