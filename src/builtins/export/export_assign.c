/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_assign.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:17:30 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 15:31:18 by alfavre          ###   ########.fr       */
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
