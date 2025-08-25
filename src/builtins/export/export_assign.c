/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_assign.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:53:12 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:53:12 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_assignment(t_exec *exec, t_ass *assign)
{
	if (!is_a_valid_identifier(assign->key))
	{
		print_error("export", assign->key, "not a valid identifier");
		return (1);
	}
	if (set_env_var(assign->key, assign->value, exec->shell->env,
			assign->is_append))
		return (0);
	else
		return (1);
}

/**
 * @brief Exports variables with value assignment.
 * 
 * @param exec Execution context containing export arguments
 * @return int 0 on success, 1 on failure
 */
int	export_with_assignment(t_exec *exec)
{
	t_ass	*head;
	int		error;

	head = exec->shell->commands->assignments;
	error = 0;
	while (head)
	{
		error |= process_assignment(exec, head);
		head = head->next;
	}
	return (error);
}
