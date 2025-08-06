/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:40:47 by alexis            #+#    #+#             */
/*   Updated: 2025/08/02 22:53:17 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Opens a file and duplicates its file descriptor to the specified standard
 * file descriptor (stdin or stdout).
 * @param file The file to open.
 * @param flags The flags to use when opening the file.
 * @param stdfd The standard file descriptor to duplicate to (STDIN_FILENO or
 * STDOUT_FILENO).
 * @return 0 on success, 1 on error.
 */
int	open_and_dup(const char *file, int flags, int stdfd)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
		return (perror(file), 1);
	if (dup2(fd, stdfd) < 0)
	{
		close(fd);
		return (perror("dup2"), 1);
	}
	close(fd);
	return (0);
}

/**
 * Duplicates a file descriptor to a standard file descriptor and closes the
 * original file descriptor.
 * @param fd The file descriptor to duplicate.
 * @param stdfd The standard file descriptor to duplicate to (STDIN_FILENO or
 * STDOUT_FILENO).
 * @param err The error message to print if dup2 fails.
 * @return 0 on success, 1 on error.
 */
int	dup_and_close(int fd, int stdfd, const char *err)
{
	if (dup2(fd, stdfd) < 0)
	{
		close(fd);
		perror(err);
		return (1);
	}
	close(fd);
	return (0);
}

void	restore_std(int saved_stdout, int saved_stdin)
{
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}
