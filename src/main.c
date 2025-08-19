/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:14:08 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/19 17:16:47 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_received = 0;

/**
 * Check if there is some arguments when minishell is launched
 * @param argc: quantity of arguments (executor's name included)
 * @param argv: char tab with all arguments (executor's name included)
 * @return: 1 if error, 0 otherwise
 */
static int	handle_arguments(int argc, char **argv)
{
	if (argc > 1)
	{
		if (argv[1][0] == '-' && argv[1][1] == 'c')
		{
			print_error("minishell", "-c", "option not supported");
			return (1);
		}
		else
		{
			print_error("minishell", argv[1], "no such file or directory");
			return (1);
		}
	}
	return (0);
}

/**
 * Initialize shell and environment
 * @param envp: Environment variables
 * @return: Initialized shell or NULL on error
 */
static t_shell	*setup_shell(char **envp)
{
	t_shell	*shell;

	shell = get_shell(envp);
	if (!shell)
		return (NULL);
	if (ft_shlvl(shell))
		set_env_var("SHLVL", "1", shell->env, 0);
	if (isatty(STDIN_FILENO))
		print_welcome();
	return (shell);
}

/**
 * Main function
 * @param argc: Argument count
 * @param argv: Argument vector
 * @param envp: Environment variables
 * @return: Exit code
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_exec	*exec;
	int		exit_code;

	if (handle_arguments(argc, argv))
		return (1);
	shell = setup_shell(envp);
	if (!shell)
		return (GENERAL_ERROR);
	parent_signal();
	run_shell_loop(shell);
	exit_code = shell->env->last_exit_status;
	exec = get_exec();
	cleanup_all(exec);
	free(exec);
	return (exit_code);
}
