/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:51:55 by alexis            #+#    #+#             */
/*   Updated: 2025/08/03 01:12:49 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_value(char **value, const char *new_value)
{
	if (*value)
		free(*value);
	*value = ft_strdup(new_value);
}

static void	append_value(char **value, const char *new_value)
{
	char	*old_value;

	if (!*value)
		*value = ft_strdup(new_value);
	else
	{
		old_value = *value;
		*value = ft_strjoin(old_value, new_value);
		free(old_value);
	}
}

/**
 * Replace or add a variable in the environment
 * @param new_value: New value for the variable
 * @param i: Index of the variable in the environment
 * @param env: Environment structure
 * @param is_append: If true, append to existing value
 * @return: 1 on success, 0 on failure
 */
int	set_env_var(char *key, char *new_value, t_env *env, int is_append)
{
	t_env_var	*var;
	t_env_var	*new_var;

	if (!new_value || !env)
		return (0);
	var = env->variables;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
			break ;
		var = var->next;
	}
	if (!var)
	{
		new_var = create_env_var(key, new_value);
		if (new_var)
		{
			add_env_var(&env->variables, new_var);
			return (1);
		}
		return (0);
	}
	if (is_append)
		append_value(&var->value, new_value);
	else
		add_value(&var->value, new_value);
	if (!var->value)
		return (0);
	return (1);
}
/*int	replace_value_var_or_add(char *new_value, int i, t_env *env, int is_append)
{
	char	*old_value;
	int		pos;

	pos = 0;
	if (!new_value || !env)
		return (0);
	if (!is_append)
	{
		free(env->variables[i].key);
		env->variables[i].key = ft_strdup(new_value);
		if (!env->variables[i].key)
			return (0);
	}
	else
	{
		old_value = env->variables[i].key;
		env->variables[i].key = ft_strjoin(old_value, new_value);
		free(old_value);
		if (!env->variables[i].key)
			return (0);
	}
	return (1);
}*/
