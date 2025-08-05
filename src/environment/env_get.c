/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 17:50:12 by alexis            #+#    #+#             */
/*   Updated: 2025/07/29 18:11:15 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Find environment variable index by key
 * @param variable: Variable name to find
 * @param env: Environment structure
 * @return: Index position in env tab or -1 if not found
 */
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

/**
 * Get environment variable value
 * @param variable: Variable name
 * @param shell: Shell structure
 * @return: Variable value or NULL if not found
 */
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
