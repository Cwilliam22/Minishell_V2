/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:36:30 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/19 18:36:44 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
