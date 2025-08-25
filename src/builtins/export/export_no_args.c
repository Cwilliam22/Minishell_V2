/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:53:27 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:53:27 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swipe(t_env_var *current, t_env_var *next_node)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = current->key;
	temp_value = current->value;
	current->key = next_node->key;
	current->value = next_node->value;
	next_node->key = temp_key;
	next_node->value = temp_value;
}

static void	sort_list(t_env_var *head)
{
	t_env_var	*current;
	t_env_var	*next_node;

	if (!head)
		return ;
	current = head;
	while (current)
	{
		next_node = current->next;
		while (next_node)
		{
			if (ft_strcmp(current->key, next_node->key) > 0)
				swipe(current, next_node);
			next_node = next_node->next;
		}
		current = current->next;
	}
}

static t_env_var	*copy_env_sorted(t_exec *exec)
{
	t_env_var	*cpy;
	t_env_var	*new_var;
	t_env_var	*head;

	head = exec->shell->env->variables;
	cpy = NULL;
	while (head)
	{
		new_var = create_env_var(head->key, head->value);
		if (!new_var)
			return (free_env_vars(cpy), NULL);
		add_env_var(&cpy, new_var);
		head = head->next;
	}
	sort_list(cpy);
	return (cpy);
}

/**
 * @brief Prints environment variables in sorted order.
 * 
 * @param exec Execution context containing environment variables
 */
void	print_env_sorted(t_exec *exec)
{
	t_env_var	*sorted_env;
	t_env_var	*current;

	sorted_env = copy_env_sorted(exec);
	if (!sorted_env)
		return ;
	current = sorted_env;
	while (current)
	{
		if (ft_strcmp(current->key, "_") != 0)
		{
			if (current->value)
				printf("declare -x %s=\"%s\"\n", current->key,
					current->value);
			else
				printf("declare -x %s\n", current->key);
		}
		current = current->next;
	}
	free_env_vars(sorted_env);
}
