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
	{
		execute_commands(shell);
	}
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
void	run_shell_loop(t_shell *shell)
{
	int	pos;

	while (shell->running)
	{
		shell->input_line = readline(PROMPT);
		if (!shell->input_line)
		{
			printf("exit\n");
			set_exit_status(SUCCESS);
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
