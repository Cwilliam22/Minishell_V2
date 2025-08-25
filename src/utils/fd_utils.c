/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:03:29 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 14:04:23 by alfavre          ###   ########.ch       */
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
int	open_and_dup(const char *file, int flags, int target_fd)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		if (errno == ENOENT)
			print_error(NULL, (char *)file, "No such file or directory");
		else if (errno == EACCES)
			print_error(NULL, (char *)file, "Permission denied");
		else if (errno == EISDIR)
			print_error(NULL, (char *)file, "Is a directory");
		else
			print_error(NULL, (char *)file, strerror(errno));
		return (1);
	}
	if (dup2(fd, target_fd) < 0)
	{
		close(fd);
		perror("dup2");
		return (1);
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

/**
* @brief Restores standard input and output file descriptors
* @param saved_stdout Previously saved stdout file descriptor
* @param saved_stdin Previously saved stdin file descriptor
*/
void	restore_std(int saved_stdout, int saved_stdin)
{
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}
