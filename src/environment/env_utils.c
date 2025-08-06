/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 07:38:09 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 18:41:39 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_env_variables(t_env *env, char **envp)
{
	t_env_var	*var;
	char		*key;
	char		*value;
	int			i;

	i = 0;
	if (!envp)
	{
		printf("no env");
		return ;
	}
	while (envp[i])
	{
		if (split_assignment(envp[i], &key, &value))
		{
			var = create_env_var(key, value);
			if (var)
			{
				add_env_var(&env->variables, var);
				env->nbr_var_env++;
			}
			free(key);
			free(value);
		}
		i++;
	}
}

/**
 * Initialize environment from system envp
 * @param envp: System environment variables
 * @return: Environment structure or NULL on error
 */
t_env	*init_environment(char **envp)
{
	t_env		*env;

	env = (t_env *)safe_malloc(sizeof(t_env));
	env->variables = NULL;
	env->nbr_var_env = 0;
	env->last_exit_status = 0;
	if (!envp || !envp[0])
	{
		init_minimal_env_vars(env);
		return (env);
	}
	init_env_variables(env, envp);
	return (env);
}

/**
 * Free environment structure
 * @param env: Environment to free
 */
void	free_environment(t_env *env)
{
	if (!env)
		return ;
	free_env_vars(env->variables);
	free(env);
}

char	**env_to_string(t_env *env)
{
	t_env_var	*current;
	char		**env_strings;
	int			i;
	char		*temp;

	if (!env || !env->variables)
		return (NULL);
	env_strings = (char **)safe_malloc((env->nbr_var_env + 1) * sizeof(char *));
	current = env->variables;
	i = 0;
	while (current)
	{
		env_strings[i] = ft_strjoin(current->key, "=");
		if (current->value)
		{
			temp = env_strings[i];
			env_strings[i] = ft_strjoin(temp, current->value);
			free(temp);
		}
		current = current->next;
		i++;
	}
	env_strings[i] = NULL;
	return (env_strings);
}
