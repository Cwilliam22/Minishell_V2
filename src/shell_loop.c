/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:57:13 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/22 10:57:13 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	tokenize_and_parse(t_shell *shell)
{
	t_token	*tokens;
	int		exit_code;

	tokens = tokenize(shell->input_line);
	if (!tokens)
		return ;
	exit_code = check_token_syntax(tokens);
	if (exit_code != 1)
	{
		free_tokens(tokens);
		set_exit_status(exit_code);
		return ;
	}
	shell->commands = parse_tokens(tokens);
	free_tokens(tokens);
}

static int	prepare_and_validate(t_shell *shell)
{
	if (has_heredocs(shell->commands))
		if (handle_heredocs(shell->commands) == -1)
			return (-1);
	expand_commands(shell);
	return (0);
}

static void	process_commands_pipeline(t_shell *shell)
{
	if (prepare_and_validate(shell) == 0)
		execute_commands(shell);
	free_commands(shell->commands);
	shell->commands = NULL;
}

void	process_input(t_shell *shell)
{
	int		pos;

	if (!shell->input_line || shell->input_line[0] == '\0')
		return ;
	pos = skip_whitespace(shell->input_line, 0);
	if (!shell->input_line[pos])
		return ;
	tokenize_and_parse(shell);
	if (shell->commands)
		process_commands_pipeline(shell);
}

/**
 * Run the main shell loop
 * @param shell: Shell structure
 */
//void	run_shell_loop(t_shell *shell)
static void	run_interactive_shell(t_shell *shell)
{
	int	pos;

	while (shell->running)
	{
		shell->input_line = readline(PROMPT);
		if (!shell->input_line)
		{
			printf("exit\n");
			break ;
		}
		handle_signal();
		if (shell->input_line && shell->input_line[0] != '\0')
		{
			pos = skip_whitespace(shell->input_line, 0);
			if (shell->input_line[pos])
			{
				add_history(shell->input_line);
				process_input(shell);
			}
		}
		cleanup_iteration(shell);
	}
}
/* Fonction pour utilsier minishell depuis un script*/
static void	run_batch_shell(t_shell *shell, char **all_lines, int line_count)
{
	int	i;
	int	pos;
	static int	heredoc_index = 0;

	i = 0;
	shell->all_lines = all_lines;      // Stockez pour les heredocs
		shell->current_line_index = &heredoc_index;
	shell->current_line_index = &heredoc_index;    // Pointeur pour que heredocs puissent avancer
	shell->total_lines = line_count;
	while (i < line_count && shell->running)
	{
		if (heredoc_index > i)
		{
			i = heredoc_index;
			continue ;
		}
		shell->input_line = all_lines[i];
		i++;
		if (shell->input_line && shell->input_line[0] != '\0')
		{
			pos = skip_whitespace(shell->input_line, 0);
			if (shell->input_line[pos]) {
				process_input(shell);
			}
		}
	}
}

/* Fonction pour le tester */
static char	**read_all_input(int *line_count)
{
	char **all_lines = NULL;
	char *line;
	int count = 0;
	int capacity = 10;

	all_lines = malloc(sizeof(char*) * capacity);
	while ((line = get_next_line(fileno(stdin))) != NULL) {
		if (count >= capacity) {
			capacity *= 2;
			all_lines = realloc(all_lines, sizeof(char*) * capacity);
		}
		all_lines[count] = ft_strtrim(line, "\n");
		free(line);
		count++;
	}
	*line_count = count;
	return (all_lines);
}

void run_shell_loop(t_shell *shell)
{
    char **all_lines = NULL;
    int line_count = 0;

    if (isatty(fileno(stdin))) {
        run_interactive_shell(shell);
    } else {
        all_lines = read_all_input(&line_count);
        run_batch_shell(shell, all_lines, line_count);
        free_array(all_lines);
    }
}
