/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexis <alexis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:27:17 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/24 15:15:43 by alexis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Pour le tester */
static void read_and_write_heredoc_batch(t_redir *redir, char **all_lines, int *current_line_index, int total_lines)
{
	(*current_line_index)++;
    while (*current_line_index < total_lines) {
        char *line = all_lines[*current_line_index];
        (*current_line_index)++;
        if (ft_strcmp(line, redir->heredoc->delimiter) == 0) {
            break ;
        }
        expand_heredoc_content(redir, line);
    }
}

static void	read_and_write_heredoc(t_redir *redir)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_error(NULL, "warning", "here-document delimited by eof");
			break ;
		}
		if (ft_strcmp(line, redir->heredoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		expand_heredoc_content(redir, line);
		free(line);
	}
}

int	error_fork(t_heredoc *hd)
{
	print_error(NULL, NULL, "fork failed");
	set_exit_status(1);
	if (hd->fd >= 0)
		close(hd->fd);
	if (hd->path)
		unlink(hd->path);
	return (0);
}

void	wait_child(pid_t last_pid)
{
	int		status;
	int		exit_status;

	exit_status = 0;
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			exit_status = EXIT_SIGINT;
		else
			exit_status = 128 + WTERMSIG(status);
	}
	set_exit_status(exit_status);
}

static void	in_child(t_redir *redir)
{
	t_shell	*shell;

	shell = get_shell(NULL);
	heredoc_child_signal();
	if (isatty(fileno(stdin)))
		read_and_write_heredoc(redir);
	else // Pour le tester
		read_and_write_heredoc_batch(redir, shell->all_lines, shell->current_line_index, shell->total_lines);
    // Sauvegarder l'index dans un fichier temporaire
    if (!isatty(fileno(stdin))) {
        FILE *f = fopen("/tmp/minishell_heredoc_index", "w");
        if (f) {
            fprintf(f, "%d", *shell->current_line_index);
            fclose(f);
        }
    }
	exit(0);
}

int	process_heredoc(t_redir *redir)
{
	t_heredoc	*hd;
	pid_t		pid;

	if (!redir || !redir->heredoc)
		return (0);
	hd = redir->heredoc;
	heredoc_parent_signal();
	pid = fork();
	if (pid < 0)
	{
		error_fork(hd);
		return (1);
	}
	if (pid == 0)
		in_child(redir);
	wait_child(pid);
	/* Pour le tester */
	if (!isatty(fileno(stdin))) {
		t_shell *shell = get_shell(NULL);
   		FILE *f = fopen("/tmp/minishell_heredoc_index", "r");
    	if (f) {
        	int new_index;
        	if (fscanf(f, "%d", &new_index) == 1) {
            	*shell->current_line_index = new_index;  // Mettre à jour la variable globale
        	}
        	fclose(f);
        	unlink("/tmp/minishell_heredoc_index");  // Nettoyer
    	}
	}
	if (hd->fd >= 0)
	{
		close(hd->fd);
		hd->fd = -1;
	}
	parent_signal();
	return (0);
}
