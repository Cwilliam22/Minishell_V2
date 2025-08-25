/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:54:14 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/25 11:54:24 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates PWD environment variable to current directory.
 * 
 * @param exec Execution context containing environment variables
 * @return int 0 on success, 1 on failure
 */
int	update_pwd(t_exec *exec)
{
	char	*new_pwd;
	int		result;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (0);
	result = set_env_var("PWD", new_pwd, exec->shell->env, 0);
	free(new_pwd);
	return (result);
}

/**
 * @brief Updates OLDPWD environment variable to previous directory.
 * 
 * @param exec Execution context containing environment variables
 * @return int 0 on success, 1 on failure
 */
int	update_oldpwd(t_exec *exec)
{
	char		*curr_pwd;
	int			result;

	curr_pwd = get_env_var("PWD", exec->shell->env);
	if (!curr_pwd)
	{
		curr_pwd = getcwd(NULL, 0);
		if (!curr_pwd)
			return (0);
	}
	result = set_env_var("OLDPWD", curr_pwd, exec->shell->env, 0);
	free(curr_pwd);
	return (result);
}

/**
 * @brief Implements the built-in 'pwd' command.
 * 
 * @param exec Execution context (unused but for consistency)
 * @return int 0 on success, 1 on failure
 */
int	builtin_pwd(t_exec *exec)
{
	char	*path;

	(void)exec;
	path = getcwd(NULL, 0);
	if (!path)
	{
		print_error("pwd", path, "The file or directory couldn't be "
			"determined");
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}
