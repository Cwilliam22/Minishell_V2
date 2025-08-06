/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:56:31 by alexis            #+#    #+#             */
/*   Updated: 2025/08/06 15:28:58 by alfavre          ###   ########.fr       */
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
	if (!shell->commands)
		return ;
	return ;
}

/**
 * Process input line through all parsing stages
 * @param shell: Shell structure
 * @return: Exit code
 */
void	process_input(t_shell *shell)
{
	int		pos;

	if (!shell->input_line || shell->input_line[0] == '\0')
		return ;
	pos = skip_whitespace(shell->input_line, 0);
	if (shell->input_line[pos])
	{
		if (!check_quotes(shell->input_line))
		{
			print_error(NULL, NULL, "unclosed quotes");
			set_exit_status(SYNTAX_ERROR);
			return ;
		}
		tokenize_and_parse(shell);
		expand_commands(shell);
		execute_commands(shell);
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

/**
 * Handle one iteration of shell input
 * @param shell: Shell structure
 * @return: 1 to continue, 0 to exit
 */
int	handle_iteration(t_shell *shell)
{
	int	pos;

	shell->input_line = readline(PROMPT);
	if (!shell->input_line)
	{
		printf("exit\n");
		set_exit_status(SUCCESS);
		return (0);
	}
	process_input(shell);
	if (shell->input_line && shell->input_line[0] != '\0')
	{
		pos = skip_whitespace(shell->input_line, 0);
		if (shell->input_line[pos])
			add_history(shell->input_line);
	}
	cleanup_iteration(shell);
	return (shell->running);
}

/**
 * Run the main shell loop
 * @param shell: Shell structure
 */
void	run_shell_loop(t_shell *shell)
{
	while (shell->running)
	{
		if (!handle_iteration(shell))
			break ;
	}
}
