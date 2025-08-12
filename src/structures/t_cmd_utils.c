/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:38:04 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 16:13:31 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Create a new command structure
 * @return: New command or NULL on error
 */
t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)safe_malloc(sizeof(t_cmd));
	cmd->args = NULL;
	cmd->args_expanded = NULL;
	cmd->assignments = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->cmd_path = NULL;
	cmd->state_path = PATH_SIMPLE;
	return (cmd);
}

/**
 * Add command to the end of the list
 * @param head: Pointer to the head of the command list
 * @param new_cmd: Command to add
 */
void	add_command(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!head || !new_cmd)
		return ;
	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

/**
 * @brief Free one command
 * @param cmd The command to free
 */
static void	free_command(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_array(cmd->args);
	if (cmd->args_expanded)
		free_array(cmd->args_expanded);
	if (cmd->assignments)
		free_assignments(cmd->assignments);
	if (cmd->redirections)
		free_redirections(cmd->redirections);
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	free(cmd);
}

/**
 * Free all commands in the list
 * @param commands: Head of the command list
 */
void	free_commands(t_cmd *commands)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!commands)
		return ;
	current = commands;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}

int	get_nb_commands(t_cmd *commands)
{
	int		i;
	t_cmd	*head;

	i = 0;
	head = commands;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}


