/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:55:39 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:55:56 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_var_in_env(char *variable, t_env *env)
{
	int			i;
	t_env_var	*current;

	if (!env || !variable)
		return (-1);
	i = 0;
	current = env->variables;
	while (current)
	{
		if (ft_strcmp(current->key, variable) == 0)
			return (i);
		current = current->next;
		i++;
	}
	return (-1);
}

static char	*find_value_in_env(char *variable, t_env *env)
{
	t_env_var	*env_var;

	if (!variable || !env)
		return (NULL);
	env_var = env->variables;
	while (env_var)
	{
		if (ft_strcmp(env_var->key, variable) == 0)
			return (ft_strdup(env_var->value));
		env_var = env_var->next;
	}
	return (NULL);
}

/**
* @brief Retrieves the value of an environment variable.
* 
* @param key Variable name to search for
* @param env Environment variables list
* @return char* Variable value if found, NULL otherwise
*/
char	*get_env_var(char *key, t_env *env)
{
	int		index;
	char	*value;

	index = find_var_in_env(key, env);
	if (index == -1)
		return (NULL);
	value = find_value_in_env(key, env);
	return (value);
}
