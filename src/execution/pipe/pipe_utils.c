/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:57:50 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/11 14:23:36 by alfavre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	kill_all_process(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGKILL);
		i++;
	}
}

int	create_pipes(t_exec *exec, int ***pipes)
{
	int	i;

	i = 0;
	*pipes = (int **)safe_malloc(sizeof(int *) * exec->nbr_pipes);
	while (i < exec->nbr_pipes)
	{
		(*pipes)[i] = (int *)safe_malloc(sizeof(int) * 2);
		if (pipe((*pipes)[i]) == -1)
			return (free_pipes(*pipes, exec), 0);
		i++;
	}
	return (1);
}

int	free_pipes(int **pipes, t_exec *exec)
{
	int	i;

	i = 0;
	while (i < exec->nbr_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	return (0);
}

void	close_pipes(int **pipes, t_exec *exec)
{
	int	j;

	j = 0;
	while (j < exec->nbr_pipes)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}
