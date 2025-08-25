/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:56:40 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:56:40 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Create a new environment variable
 * @param key: Variable name
 * @param value: Variable value
 * @return: New environment variable or NULL on error
 */
t_env_var	*create_env_var(char *key, char *value)
{
	t_env_var	*var;

	var = (t_env_var *)safe_malloc(sizeof(t_env_var));
	var->key = ft_strdup(key);
	if (!var->key)
	{
		free(var);
		return (NULL);
	}
	var->value = ft_strdup(value);
	var->next = NULL;
	return (var);
}

/**
 * Add environment variable to the list
 * @param head: Pointer to the head of the list
 * @param new_var: Variable to add
 */
void	add_env_var(t_env_var **head, t_env_var *new_var)
{
	t_env_var	*current;

	if (!head || !new_var)
		return ;
	if (!*head)
	{
		*head = new_var;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_var;
}

/**
* @brief Removes an environment variable from the list.
* 
* @param key Variable name to remove
* @param env Environment variables list
* @return int 0 on success, 1 on failure or not found
*/
int	remove_env_var(char *key, t_env *env)
{
	t_env_var	**current;
	t_env_var	*to_delete;

	if (!key || !env)
		return (0);
	current = &env->variables;
	while (*current)
	{
		if (ft_strcmp((*current)->key, key) == 0)
		{
			to_delete = *current;
			*current = (*current)->next;
			free(to_delete->key);
			free(to_delete->value);
			free(to_delete);
			env->nbr_var_env--;
			return (1);
		}
		current = &(*current)->next;
	}
	return (0);
}

/**
 * Free a single environment variable
 * @param var: Variable to free
 */
void	free_env_var(t_env_var *var)
{
	if (!var)
		return ;
	if (var->key)
		free(var->key);
	if (var->value)
		free(var->value);
	free(var);
}

/**
 * Free all environment variables in the list
 * @param vars: Head of the variable list
 */
void	free_env_vars(t_env_var *vars)
{
	t_env_var	*current;
	t_env_var	*next;

	current = vars;
	while (current)
	{
		next = current->next;
		free_env_var(current);
		current = next;
	}
}
