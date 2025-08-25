/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:22:59 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 13:23:04 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates and initializes an execution structure
 * @param shell Pointer to the shell structure
 * @return Pointer to the new t_exec structure
 */
t_exec	*create_exec(t_shell *shell)
{
	t_exec	*exec;

	if (!shell)
		return (NULL);
	exec = (t_exec *)safe_malloc(sizeof(t_exec));
	exec->shell = shell;
	exec->current_cmd = NULL;
	exec->pids = NULL;
	exec->path = get_env_var("PATH", shell->env);
	exec->env_copy = env_to_string(shell->env);
	if (!exec->env_copy)
		return (free_exec(exec), NULL);
	exec->nb_arg = 0;
	exec->nb_var_env = shell->env->nbr_var_env;
	exec->nb_process = get_nb_commands(shell->commands);
	exec->nbr_pipes = exec->nb_process - 1;
	if (exec->nb_process > 1)
		exec->is_pipe = 1;
	else
		exec->is_pipe = 0;
	return (exec);
}

/**
 * @brief Frees memory of an execution structure
 * @param exec Structure to free
 */
void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->pids)
		free(exec->pids);
	if (exec->path)
		free(exec->path);
	if (exec->env_copy)
		free_array(exec->env_copy);
	free(exec);
}
