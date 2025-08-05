/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:02:24 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/03 00:14:19 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//anciennement find_var_path
char	*get_path(t_env *env)
{
	char		*path;
	t_env_var	*head;

	if (!env)
		return (NULL);
	head = env->variables;
	while (head)
	{
		if (head->key && head->value && ft_strcmp(head->key, "PATH") == 0)
		{
			path = ft_strdup(head->value);
			if (!path)
				cleanup_and_exit(2);
			return (path);
		}
		head = head->next;
	}
	return (NULL);
}

//anciennement var_path
void	update_var_path(t_exec *exec)
{
	if (exec->path)
	{
		free(exec->path);
		exec->path = NULL;
	}
	exec->path = get_path(exec->shell->env);
}



