/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:57:50 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/21 23:37:26 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_all_pipeline_children(pid_t *pids, int nb_process)
{
	int	i;
	int	status;
	int	last_exit_code;

	last_exit_code = 0;
	i = 0;
	while (i < nb_process)
	{
		waitpid(pids[i], &status, 0);
		if (i == nb_process - 1)
		{
			if (WIFEXITED(status))
				last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
	set_exit_status(last_exit_code);
}
