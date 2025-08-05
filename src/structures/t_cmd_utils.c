/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:38:04 by alexis            #+#    #+#             */
/*   Updated: 2025/08/04 18:37:26 by alfavre          ###   ########.fr       */
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
 * Free all commands in the list
 * @param commands: Head of the command list
 */
void	free_commands(t_cmd *commands)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

	current = commands;
	while (current)
	{
		next = current->next;
		if (current->args)
		{
			i = 0;
			while (current->args[i])
			{
				free(current->args[i]);
				i++;
			}
			free(current->args);
		}
		free_assignments(current->assignments);
		free_redirections(current->redirections);
		free(current->cmd_path);
		current->cmd_path = NULL;
		free(current);
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

int	get_nb_command_args(t_cmd *commands)
{
	char	**args;
	int		i;

	args = commands->args;
	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}
