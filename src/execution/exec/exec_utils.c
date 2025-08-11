/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:06:26 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/06 15:19:19 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	*create_exec(t_shell *shell)
{
	t_exec	*exec;

	exec = (t_exec *)safe_malloc(sizeof(t_exec));
	exec->shell = shell;
	exec->current_cmd = NULL;
	exec->pipe_fds = NULL;
	exec->path = get_env_var("PATH", exec->shell->env);
	exec->env_copy = NULL;
	exec->nb_arg = 0;
	exec->is_pipe = 0;
	exec->saved_stdin = -1;
	exec->saved_stdout = -1;
	exec->saved_stderr = -1;
	exec->nb_var_env = exec->shell->env->nbr_var_env;
	exec->nb_process = get_nb_commands(exec->shell->commands);
	exec->nbr_pipes = exec->nb_process - 1;
	return (exec);
}

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->pipe_fds)
		free(exec->pipe_fds);
	if (exec->path)
		free(exec->path);
	if (exec->env_copy)
		free_array(exec->env_copy);
	free(exec);
}

void	free_var(t_exec *exec)
{
	if (exec->path)
		free(exec->path);
}
